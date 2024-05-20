using CLR;
using CodeMetaData;
using CodeMetaDataComparator;
using FileStorageSystem.FileId;
using System.CodeDom;
using System.Collections.Immutable;
using System.ComponentModel;
using System.Globalization;
using System.IO;
using System.Runtime.CompilerServices;
using System.Windows;
using WpfCodeMetaDataProcessorVisualization.UserControls;

namespace WpfCodeMetaDataProcessorVisualization.ViewModels
{
    public static class NotificationTexts
    {
        public static readonly string noLoaded = "No Loaded File";
        public static readonly string invalidInput = "Invalid input";
        public static readonly string noChoose = "No Choose File";
        public static readonly string noChooseOrLoad = "No Choose or Load File";
        public static readonly string noChoosePrecompareOrLoad = "No Choose Precompare or Load";
        public static readonly string resultCompare = "Result compare: ";
        public static readonly string resultFullCompare = "See result in output dir";
    }

    public sealed class CodeMetaDataProcessorApplication : INotifyPropertyChanged
    {
        private readonly string _initFileStorageName = "System.txt";
        private float param = (float)0.5;

        private FileStorageSytemViewModel viewModelFileSystem;
        private FileMetaData loadedMetaData;
        private FileMetaData choosenMetaData;
        private FileId choosenFullCompareFileId;
        private FileId choosenCompareFileId;

        public float Parametr { get { return param; } set { param = value; } }
        public ImmutableDictionary<FileId, FileMetaData> GetSystem { get { if(viewModelFileSystem == null) 
                    return default; return viewModelFileSystem.GetSystem; } }
        public string LoadMetaDataText { get { if (loadedMetaData == null) { return NotificationTexts.noLoaded; } 
                return new CodeMetaDataSerializer.JsonCodeMetaDataSerializer().SerializeFileMetaData(new CodeMetaDataConverter.MetaDataConverter().ConvertFileMetaDataToDto(loadedMetaData));
            }
            set { } }
        public string ChooseMetaDataText { get { if (choosenMetaData == null) { return NotificationTexts.noChoose; } 
                return new CodeMetaDataSerializer.JsonCodeMetaDataSerializer().SerializeFileMetaData(new CodeMetaDataConverter.MetaDataConverter().ConvertFileMetaDataToDto(choosenMetaData)); } set { } }
        public string CompareMetaDataText { get { if (loadedMetaData == null || choosenMetaData == null) 
                { return NotificationTexts.noChooseOrLoad; } return NotificationTexts.resultCompare + ComparerMetaData.CompareFileMetaData(loadedMetaData, choosenMetaData).ToString(); } set { } }
        public string FullCompareMetaDataText { get { if (loadedMetaData == null || choosenFullCompareFileId == null) 
                { return NotificationTexts.noChoosePrecompareOrLoad; } return NotificationTexts.resultFullCompare; } set { } }
        public List<FileId> PrecompareCandidates { get { if (viewModelFileSystem == null || loadedMetaData == null) { return new(); } 
                return viewModelFileSystem.GetFullCompareCanditates(loadedMetaData, param); } }


        public CodeMetaDataProcessorApplication()
        {
            InitSystem();
        }

        private async void InitSystem()
        {
            viewModelFileSystem = await FileStorageSytemViewModel.CreateAsync(new System.IO.FileInfo(_initFileStorageName));
            OnPropertyChanged(nameof(GetSystem));
        }

        public void SetParametrHandler(object sender, EventArgs e)
        {
            DialogWindow dialogWindow = new DialogWindow();

            if (dialogWindow.ShowDialog() == true)
            {
                float number;
                bool success = float.TryParse(dialogWindow.Text, out number);

                if(success)
                {
                    param = number;
                    OnPropertyChanged(nameof(Parametr));
                }
                else
                {
                    MessageBox.Show(NotificationTexts.invalidInput);
                }
            }
            else
            {
                MessageBox.Show("Canceled");
            }
        }

        public async void ButtonLoadHandler(object sender, EventArgs e)
        {
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();

            bool? response = dlg.ShowDialog();

            if (response == true)
            {
                FileMetaData metaData = null;

                try
                {
                    metaData = CMCDFacadeWrapper.ProccesFunctionsCodeMetaData(new FileInfo(dlg.FileName));
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                    return;
                }

                loadedMetaData = metaData;
                choosenCompareFileId = new FileId(new FileInfo(dlg.FileName));
                viewModelFileSystem.AddWithMetaData(dlg.FileName, metaData);

                OnPropertyChanged(nameof(LoadMetaDataText));
                OnPropertyChanged(nameof(GetSystem));
            }
        }

        public void ListViewItem_ChoosenFileFromStorageHandler(object sender, EventArgs e)
        {
            var item = sender as FileStorageView;

            if (item != null)
            {
                var selectedItem = item.GetSelectedItem();
                choosenMetaData = selectedItem.Value;
                OnPropertyChanged(nameof(ChooseMetaDataText));
            }
        }

        public void ListViewItem_ChoosenCandidateFileHandler(object sender, EventArgs e)
        {
            var item = sender as PrecompareCandidatesView;

            if (item != null && item.SelectedItem != null)
            {
                var selectedItem = item.SelectedItem;
                choosenFullCompareFileId = selectedItem;
            }
        }

        public void ButtonCompareHandler(object sender, EventArgs e)
        {
            try
            {
                ComparerMetaData.CompareFileMetaData(loadedMetaData, choosenMetaData).ToString();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                return;
            }

            OnPropertyChanged(nameof(CompareMetaDataText));
        }

        public void ButtonFullCompareHandler(object sender, EventArgs e)
        {
            try
            {
                int argc = 2;
                string[] argv = [choosenFullCompareFileId.Id, choosenCompareFileId.Id];
                funcs_clones_analysis_through_cm_app_clr.Exec(argc, argv);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                return;
            }

            OnPropertyChanged(nameof(FullCompareMetaDataText));
        }

        public void ButtonGetCandidatedPrecompareHandler(object sender, EventArgs e)
        {
            try
            {
                viewModelFileSystem.GetFullCompareCanditates(loadedMetaData, param);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
                return;
            }
            OnPropertyChanged(nameof(PrecompareCandidates));
        }

        public void ClosingWindow(object sender, EventArgs e)
        {
            ClosingDialogWindow dialogWindow = new();

            if (dialogWindow.ShowDialog() == true)
            {
                StreamWriter writer = new StreamWriter(_initFileStorageName);
                viewModelFileSystem.Save(writer);
            }
            else
            {
                MessageBox.Show("Canceled");
            }
        }

        public event PropertyChangedEventHandler? PropertyChanged;
        protected void OnPropertyChanged([CallerMemberName] string property = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(property));
        }
    }
}

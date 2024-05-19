using CLR;
using CodeMetaData;
using CodeMetaDataComparator;
using FileStorageSystem.FileId;
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
        public static string noLoaded = "No Loaded File";
        public static string invalidInput = "Invalid input";
        public static string noChoose = "No Choose File";
        public static string noChooseOrLoad = "No Choose or Load File";
        public static string noChoosePrecompareOrLoad = "No Choose Precompare or Load";
    }

    public sealed class CodeMetaDataProcessorApplication : INotifyPropertyChanged
    {
        private string _initFileStorageName = "System.txt";
        private float param = (float)0.5;

        private FileStorageSytemViewModel viewModelFileSystem;
        private FileMetaData loadedMetaData;
        private FileMetaData choosenMetaData;
        private FileMetaData choosenFullCompareMetaData;

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
                { return NotificationTexts.noChooseOrLoad; } return ComparerMetaData.CompareFileMetaData(loadedMetaData, choosenMetaData).ToString(); } set { } }
        public string FullCompareMetaDataText { get { if (loadedMetaData == null || choosenFullCompareMetaData == null) 
                { return NotificationTexts.noChoosePrecompareOrLoad; } return ComparerMetaData.CompareFileMetaData(choosenFullCompareMetaData, loadedMetaData).ToString(); } set { } }
        public List<FileId> PrecompareCandidates { get { if (viewModelFileSystem == null || loadedMetaData == null) return new(); 
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

        public void setParametrHandler(object sender, EventArgs e)
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

        public void GetCandidatedPrecompareHandler(object sender, EventArgs e)
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

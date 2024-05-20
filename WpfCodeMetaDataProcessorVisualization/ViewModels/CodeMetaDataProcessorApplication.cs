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
        public static readonly string wasNotLoad = "No Loaded File";
        public static readonly string invalidInput = "Invalid input";
        public static readonly string wasNotChoose = "No Choose File";
        public static readonly string wasNotChooseOrLoad = "No Choose or Load File";
        public static readonly string wasNotChoosePrecompareOrLoad = "No Choose Precompare or Load";
        public static readonly string resultCompare = "Result compare: ";
        public static readonly string resultFullCompare = "See result in \".\\results\\Result.txt\"";
    }

    public sealed class CodeMetaDataProcessorApplication : INotifyPropertyChanged
    {
        private readonly string _initFileStorageFolder = "System.txt";
        private readonly string _configName = "config.txt";

        private readonly string _fullCompareResultFolder = "<Output> \".\\results\\Result.txt\"";

        private float param = (float)0.5;

        int outDirPos = 0;
        int secondDirPos = 3;

        private FileStorageSytemViewModel viewModelFileSystem;
        private FileMetaData loadedMetaData;
        private FileMetaData choosenMetaData;

        private FileId choosenFullCompareFileId;
        private FileId loadedCompareFileId;

        public float Parametr { get { return param; } set { param = value; } }
        public ImmutableDictionary<FileId, FileMetaData> GetSystem { get { if(viewModelFileSystem == null) 
                    return default; return viewModelFileSystem.GetSystem; } }
        public string LoadMetaDataText { get { if (loadedMetaData == null) { return NotificationTexts.wasNotLoad; } 
                return new CodeMetaDataSerializer.JsonCodeMetaDataSerializer().SerializeFileMetaData(new CodeMetaDataConverter.MetaDataConverter().ConvertFileMetaDataToDto(loadedMetaData));} set { } }
        public string ChooseMetaDataText { get { if (choosenMetaData == null) { return NotificationTexts.wasNotChoose; } 
                return new CodeMetaDataSerializer.JsonCodeMetaDataSerializer().SerializeFileMetaData(new CodeMetaDataConverter.MetaDataConverter().ConvertFileMetaDataToDto(choosenMetaData)); } set { } }
        public string CompareMetaDataText { get { if (loadedMetaData == null || choosenMetaData == null) 
                { return NotificationTexts.wasNotChooseOrLoad; } return NotificationTexts.resultCompare + ComparerMetaData.CompareFileMetaData(loadedMetaData, choosenMetaData).ToString(); } set { } }
        public string FullCompareMetaDataText { get { if (loadedMetaData == null || choosenFullCompareFileId == null) 
                { return NotificationTexts.wasNotChoosePrecompareOrLoad; } return NotificationTexts.resultFullCompare; } set { } }
        public List<FileId> PrecompareCandidates { get { if (viewModelFileSystem == null || loadedMetaData == null) { return new(); } 
                return viewModelFileSystem.GetFullCompareCanditates(loadedMetaData, param); } }

        
        private void makeConfig()
        {
            StreamReader reader = new StreamReader(choosenFullCompareFileId.Id);
            StreamReader reader_loaded = new StreamReader(loadedCompareFileId.Id);

            string newFolder = (string)choosenFullCompareFileId.Id.Clone();

            newFolder = newFolder.Replace(choosenFullCompareFileId.ShortId, _configName);

            StreamWriter writer = new StreamWriter(newFolder);

            string line, line_1;
            int i = 0;

            while ((line = reader.ReadLine()) != null)
            {
                line_1 = reader_loaded.ReadLine();

                if (i == outDirPos)
                {
                    line = _fullCompareResultFolder;
                }
                if (i == secondDirPos)
                {
                    line = line_1;
                }

                writer.WriteLine(line);

                i++;
            }
            reader.Close();
            reader_loaded.Close();
            writer.Close();
        }

        public CodeMetaDataProcessorApplication()
        {
            InitSystem();
        }

        private async void InitSystem()
        {
            viewModelFileSystem = await FileStorageSytemViewModel.CreateAsync(new System.IO.FileInfo(_initFileStorageFolder));
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
                loadedCompareFileId = new FileId(new FileInfo(dlg.FileName));
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
                makeConfig();

                string newFolder = (string)choosenFullCompareFileId.Id.Clone();
                newFolder = newFolder.Replace(choosenFullCompareFileId.ShortId, _configName);

                string[] argv = [" ", newFolder];
                funcs_clones_analysis_through_cm_app_clr.Exec(argc, argv);

                var window = new CompareResultWindow();

                if (window.ShowDialog() == true)
                {

                }
                else
                {
                    MessageBox.Show("Canceled");
                }
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
                StreamWriter writer = new StreamWriter(_initFileStorageFolder);
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

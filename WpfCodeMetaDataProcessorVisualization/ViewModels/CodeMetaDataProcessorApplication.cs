using CLR;
using CodeMetaData;
using CodeMetaDataComparator;
using FileStorageSystem.FileId;
using System.Collections.Immutable;
using System.ComponentModel;
using System.IO;
using System.Runtime.CompilerServices;
using System.Windows;
using WpfCodeMetaDataProcessorVisualization.UserControls;
using WpfCodeMetaDataProcessorVisualization.ConstantsAndNotifications;

namespace WpfCodeMetaDataProcessorVisualization.ViewModels
{
    public sealed class CodeMetaDataProcessorApplication : INotifyPropertyChanged
    {
        private readonly string _initFileStorageFolder = "System.txt";

        private float _compareParam = (float)0.4;

        private FileStorageSytemViewModel viewModelFileSystem;

        private FileMetaData loadedMetaData;
        private FileMetaData choosenMetaData;

        private FileId choosenFullCompareFileId;
        private FileId choosenCompareFileId;

        public float CompareParametr { get { return _compareParam; } set { _compareParam = value; } }
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
        public List<FileId> PrecompareCandidates { get { if (viewModelFileSystem == null || choosenMetaData == null) { return new(); } 
                return viewModelFileSystem.GetFullCompareCanditates(choosenMetaData, _compareParam); } }

        private void makeConfig()
        {
            using StreamReader reader_settings = new StreamReader(ConfigConstants._settingsConfigName);

            using StreamWriter writer = new StreamWriter(ConfigConstants._configName);

            string line;
            int i = 0;

            while ((line = reader_settings.ReadLine()) != null)
            {
                if (i == ConfigConstants._outDirPos)
                {
                    line = ConfigConstants._fullCompareResultFolder;
                }
                if(i == ConfigConstants._firstDirPos)
                {
                    line = ConfigConstants._firstDirName + " \"" + makeAstDirectoryName(new FileInfo(choosenCompareFileId.ShortId)) + "\"";
                }
                if (i == ConfigConstants._secondDirPos)
                {
                    line = ConfigConstants._secondDirName + " \"" + makeAstDirectoryName(new FileInfo(choosenFullCompareFileId.ShortId)) + "\"";
                }

                writer.WriteLine(line);

                i++;
            }
            reader_settings.Close();
            writer.Close();
        }

        private string makeAstName(FileInfo inputFileInfo)
        { 
            string fileName = inputFileInfo.Name;

            string newFileName = fileName.Replace(".cpp", ".ast");

            return newFileName;
        }

        private string makeAstDirectoryName(FileInfo inputFileInfo)
        {
            string fileName = inputFileInfo.Name;

            string newFileFolder = fileName.Replace(".cpp", ConfigConstants._astFileFolderAdding);

            return newFileFolder;
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
                    _compareParam = number;
                    OnPropertyChanged(nameof(CompareParametr));
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
                    var info = new FileInfo(dlg.FileName);

                    var ast_name = makeAstName(info);

                    var ast_directory = makeAstDirectoryName(info);

                    AstDumper.dumpFile(info, ast_directory, ast_name);

                    metaData = CMCDFacadeWrapper.ProccesFunctionsCodeMetaData(ast_directory);
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
                choosenCompareFileId = selectedItem.Key;
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

                string[] argv = [" ", ConfigConstants._configName];
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
                viewModelFileSystem.GetFullCompareCanditates(choosenMetaData, _compareParam);
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
                using StreamWriter writer = new StreamWriter(_initFileStorageFolder);
                viewModelFileSystem.Save(writer);
                writer.Close();
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

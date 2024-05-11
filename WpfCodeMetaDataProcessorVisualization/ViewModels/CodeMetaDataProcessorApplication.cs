using CodeMetaData;
using CodeMetaDataComparator;
using FileStorageSystem.FileId;
using System.Collections.Immutable;
using System.ComponentModel;
using System.Runtime.CompilerServices;
using System.Windows;
using WpfCodeMetaDataProcessorVisualization.UserControls;
using static System.Runtime.InteropServices.JavaScript.JSType;

namespace WpfCodeMetaDataProcessorVisualization.ViewModels
{
    public static class NotificationTexts
    {
        public static string noLoaded = "No Loaded File";
        public static string invalidInput = "Invalid input";
        public static string noChoose = "No Choose File";
        public static string noChooseOrLoad = "No Choose or Load File";
        public static string noChoosePrecompareOrLoad = "No Choose Precompare or Load File";
    }

    public sealed class CodeMetaDataProcessorApplication : INotifyPropertyChanged
    {
        private string _initFileStorageName = "System.txt";
        private float param = (float)0.5;

        private FileStorageSytemViewModel viewModelFileSystem;
        private SourceCodeMetaData loadedMetaData;
        private SourceCodeMetaData choosenMetaData;

        public string ReloadFileName { get { return _initFileStorageName; } set { _initFileStorageName = value; InitSystem(); } }
        public float Parametr { get { return param; } set { param = value; } }
        public ImmutableDictionary<FileId, SourceCodeMetaData> GetSystem { get { if(viewModelFileSystem == null) return default; return viewModelFileSystem.GetSystem; } }
        public string LoadMetaDataText { get { if (loadedMetaData == null) { return NotificationTexts.noLoaded; } return loadedMetaData.ToString(); } set { } }
        public string ChooseMetaDataText { get { if (choosenMetaData == null) { return NotificationTexts.noChoose; } return choosenMetaData.ToString(); } set { } }
        public string CompareMetaDataText { get { if (loadedMetaData == null || choosenMetaData == null) { return NotificationTexts.noChooseOrLoad; } return ComparerMetaData.CompareMetaData(choosenMetaData, loadedMetaData).ToString(); } set { } }
        public List<FileId> PrecompareCandidates { get { if (viewModelFileSystem == null || loadedMetaData == null) return new(); return viewModelFileSystem.GetFullCompareCanditates(loadedMetaData, param); } }


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
                viewModelFileSystem.Add(dlg.FileName);
            }

            var metaData = await FileCodeMetaDataHandler.CreatorCodeMetaDataFromFile.MakeCodeMetaDataFromSourceFile(dlg.FileName);
            loadedMetaData = metaData;

            OnPropertyChanged(nameof(LoadMetaDataText));
            OnPropertyChanged(nameof(GetSystem));
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
            if (choosenMetaData != null && loadedMetaData != null)
            {
                try
                {
                    ComparerMetaData.CompareMetaData(choosenMetaData, loadedMetaData).ToString();
                    OnPropertyChanged(nameof(CompareMetaDataText));
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
        }

        public void GetCandidatedPrecompareHandler(object sender, EventArgs e)
        {
            try
            {
                //
                viewModelFileSystem.GetFullCompareCanditates(loadedMetaData, param);
                OnPropertyChanged(nameof(PrecompareCandidates));
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        public event PropertyChangedEventHandler? PropertyChanged;
        protected void OnPropertyChanged([CallerMemberName] string property = "")
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(property));
        }
    }
}

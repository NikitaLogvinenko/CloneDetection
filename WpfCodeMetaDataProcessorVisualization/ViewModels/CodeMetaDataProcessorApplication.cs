using CodeMetaData;
using CodeMetaDataComparator;
using FileStorageSystem.FileId;
using System;
using System.Collections.Generic;
using System.Collections.Immutable;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using WpfCodeMetaDataProcessorVisualization.UserControls;

namespace WpfCodeMetaDataProcessorVisualization.ViewModels
{
    public class CodeMetaDataProcessorApplication : INotifyPropertyChanged
    {
        private string _initFileStorageName = "System.txt";
        private float param = (float)0.5;

        private FileStorageSytemViewModel viewModelFileSystem;
        private SourceCodeMetaData loadedMetaData;
        private SourceCodeMetaData choosenMetaData;

        public ImmutableDictionary<FileId, SourceCodeMetaData> GetSystem { get { if(viewModelFileSystem == null) return default(ImmutableDictionary<FileId, SourceCodeMetaData>); return viewModelFileSystem.GetSystem; } }
        public string LoadMetaDataText { get { if (loadedMetaData == null) { return "No Loaded File"; } return loadedMetaData.ToString(); } set { } }
        public string ChooseMetaDataText { get { if (choosenMetaData == null) { return "No Choose File"; } return choosenMetaData.ToString(); } set { } }
        public string CompareMetaDataText { get { if (loadedMetaData == null || choosenMetaData == null) { return "No Choose or Load File"; } return ComparerMetaData.CompareMetaData(choosenMetaData, loadedMetaData).ToString(); } set { } }
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
            //
            //LoadCodeMetaData.Text = metaData.ToString();
            OnPropertyChanged(nameof(LoadMetaDataText));
            OnPropertyChanged(nameof(GetSystem));
        }

        public void ListViewItem_PreviewMouseLeftButtonDown(object sender, EventArgs e)
        {
            var item = sender as FileStorageView;

            if (item != null)
            {
                var selectedItem = item.GetSelectedItem();
                choosenMetaData = selectedItem.Value;
                //
                //ChooseCodeMetaData.Text = "Selected: " + selectedItem.Value;
                OnPropertyChanged(nameof(ChooseMetaDataText));
            }
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

using System.Windows;
using WpfCodeMetaDataProcessorVisualization.ViewModels;

namespace WpfCodeMetaDataProcessorVisualization
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        public MainWindow()
        {
            InitializeComponent();

            CodeMetaDataProcessorApplication application = new CodeMetaDataProcessorApplication();

            DataContext = application;

            fileLoader.ButtonLoadClick += application.ButtonLoadHandler;
            storageView.ChooseElementClick += application.ListViewItem_ChoosenFileFromStorageHandler;
            compareMetaData.ButtonCompareClick += application.ButtonCompareHandler;
            getPrecompareCandidates.GetCandidatesCompareClick += application.GetCandidatedPrecompareHandler;
            fullCompareFiles.ButtonCompareFilesClick += application.ListViewItem_ChoosenCandidateFileHandler;
            menuSettings.SetCompareParamClick += application.setParametrHandler;

            Closing += application.ClosingWindow;
        }
    }
}
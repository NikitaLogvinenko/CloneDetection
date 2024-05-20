using CodeMetaData;
using FileStorageSystem.FileId;
using System.Windows;
using System.Windows.Controls;

namespace WpfCodeMetaDataProcessorVisualization.UserControls
{
    /// <summary>
    /// Interaction logic for GetAllConditatesView.xaml
    /// </summary>
    public partial class GetAllConditatesView : UserControl
    {
        public GetAllConditatesView()
        {
            InitializeComponent();
        }

        public event RoutedEventHandler GetCandidatesCompareClick;

        private void GetCandidatesButtonClick(object sender, RoutedEventArgs e)
        {
            GetCandidatesCompareClick?.Invoke(this, e);
        }
    }
}

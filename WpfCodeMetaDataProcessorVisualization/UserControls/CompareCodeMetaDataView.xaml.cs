using System.Windows;
using System.Windows.Controls;

namespace WpfCodeMetaDataProcessorVisualization.UserControls
{
    /// <summary>
    /// Interaction logic for CompareCodeMetaDataView.xaml
    /// </summary>
    public partial class CompareCodeMetaDataView : UserControl
    {
        public CompareCodeMetaDataView()
        {
            InitializeComponent();
        }

        public event RoutedEventHandler ButtonCompareClick;

        private void CompareButtonClick(object sender, RoutedEventArgs e)
        {
            ButtonCompareClick?.Invoke(this, e);
        }
    }
}

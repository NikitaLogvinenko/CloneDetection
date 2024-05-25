using System.Windows;
using System.Windows.Controls;

namespace WpfCodeMetaDataProcessorVisualization.UserControls
{
    /// <summary>
    /// Interaction logic for LoadFileView.xaml
    /// </summary>
    public partial class LoadFileView : UserControl
    {
        public LoadFileView()
        {
            InitializeComponent();
        }

        public event RoutedEventHandler ButtonLoadClick;

        private void LoadButtonClick(object sender, RoutedEventArgs e)
        {
            ButtonLoadClick?.Invoke(this, e);
        }
    }
}

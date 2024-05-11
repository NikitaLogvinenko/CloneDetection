using System.Windows;
using System.Windows.Controls;

namespace WpfCodeMetaDataProcessorVisualization.UserControls
{
    /// <summary>
    /// Interaction logic for MenuView.xaml
    /// </summary>
    public partial class MenuView : UserControl
    {
        public MenuView()
        {
            InitializeComponent();
        }

        public event RoutedEventHandler SetCompareParamClick;

        private void SetCompareParamButtonClick(object sender, RoutedEventArgs e)
        {
            SetCompareParamClick?.Invoke(this, e);
        }

        public event RoutedEventHandler SetInitFileNameClick;

        private void SetLoadFileClick(object sender, RoutedEventArgs e)
        {
            SetInitFileNameClick?.Invoke(this, e);
        }

        public event RoutedEventHandler SaveSystem;

        private void SaveFileSystemClick(object sender, RoutedEventArgs e)
        {
            SaveSystem?.Invoke(this, e);
        }
    }
}

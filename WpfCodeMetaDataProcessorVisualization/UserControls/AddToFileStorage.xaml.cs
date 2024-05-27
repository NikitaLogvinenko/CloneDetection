using System.Windows;
using System.Windows.Controls;

namespace WpfCodeMetaDataProcessorVisualization.UserControls
{
    /// <summary>
    /// Interaction logic for AddToFileStorage.xaml
    /// </summary>
    public partial class AddToFileStorage : UserControl
    {
        public AddToFileStorage()
        {
            InitializeComponent();
        }

        public event RoutedEventHandler ButtonAddClick;

        private void AddButtonClick(object sender, RoutedEventArgs e)
        {
            ButtonAddClick?.Invoke(this, e);
        }
    }
}

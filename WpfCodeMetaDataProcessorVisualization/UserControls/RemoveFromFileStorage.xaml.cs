using System.Windows;
using System.Windows.Controls;

namespace WpfCodeMetaDataProcessorVisualization.UserControls
{
    /// <summary>
    /// Interaction logic for RemoveFromFileStorage.xaml
    /// </summary>
    public partial class RemoveFromFileStorage : UserControl
    {
        public RemoveFromFileStorage()
        {
            InitializeComponent();
        }

        public event RoutedEventHandler ButtonRemoveClick;

        private void RemoveButtonClick(object sender, RoutedEventArgs e)
        {
            ButtonRemoveClick?.Invoke(this, e);
        }
    }
}

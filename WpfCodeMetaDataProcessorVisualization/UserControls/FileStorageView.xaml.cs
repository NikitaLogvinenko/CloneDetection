using CodeMetaData;
using FileStorageSystem.FileId;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace WpfCodeMetaDataProcessorVisualization.UserControls
{
    /// <summary>
    /// Interaction logic for FileStorageView.xaml
    /// </summary>
    public partial class FileStorageView : UserControl
    {
        private KeyValuePair<FileId, SourceCodeMetaData> _selectedItem;
        public FileStorageView()
        {
            InitializeComponent();
        }

        public event RoutedEventHandler ChooseElementClick;

        public KeyValuePair<FileId, SourceCodeMetaData> GetSelectedItem()
        {
            return _selectedItem;
        }

        private void PreviewMouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            ListView view = sender as ListView;

            if (view != null)
            {
                _selectedItem = (KeyValuePair<FileId, SourceCodeMetaData>)view.SelectedItem;
            }

            ChooseElementClick?.Invoke(this, e);
        }
    }
}

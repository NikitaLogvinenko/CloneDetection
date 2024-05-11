using FileStorageSystem.FileId;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Input;

namespace WpfCodeMetaDataProcessorVisualization.UserControls
{
    /// <summary>
    /// Interaction logic for PrecompareCandidatesView.xaml
    /// </summary>
    public partial class PrecompareCandidatesView : UserControl
    {
        FileId _selectedItem;
        public FileId SelectedItem => _selectedItem;

        public PrecompareCandidatesView()
        {
            InitializeComponent();
        }

        public event RoutedEventHandler ChooseCandidateElementClick;

        private void MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            ListView view = sender as ListView;

            if (view != null)
            {
                _selectedItem = (FileId)view.SelectedItem;
            }

            ChooseCandidateElementClick?.Invoke(this, e);
        }
    }
}

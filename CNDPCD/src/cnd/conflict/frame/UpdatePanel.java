package cnd.conflict.frame;

import java.awt.Component;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.List;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.DefaultCellEditor;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JOptionPane;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.border.Border;
import javax.swing.event.TableModelEvent;
import javax.swing.event.TableModelListener;
import javax.swing.table.AbstractTableModel;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.TableCellRenderer;
import javax.swing.table.TableColumn;

import cnd.conflict.dao.PolicyDAO;
import cnd.conflict.entity.CndPolicy;

public class UpdatePanel extends JPanel implements ActionListener,
		TableModelListener {
	private PolicyDAO policyDAO = new PolicyDAO();
	private boolean DEBUG = false;

	private Box boxH, boxV;

	private JTable table = null;
	private JButton buttonUpdate;
	// private JButton buttonRefresh;
	private MyTableModel myTableModel;
	private JScrollPane scrollPane;

	public UpdatePanel() {
		super(new GridLayout(1, 0));

		changeOnJTable();

		buttonUpdate = new JButton("Update chosen policies");
		// buttonRefresh = new JButton("Refresh");

		buttonUpdate.addActionListener(this);
		// buttonRefresh.addActionListener(this);

		boxV = Box.createVerticalBox();
		boxH = Box.createHorizontalBox();

		boxH.add(Box.createHorizontalStrut(500));
		boxH.add(buttonUpdate);
		boxH.add(Box.createHorizontalStrut(10));
		// boxH.add(buttonRefresh);

		boxV.add(Box.createVerticalStrut(20));
		boxV.add(scrollPane);
		// boxV.add(Box.createVerticalStrut(30));
		boxV.add(boxH);

		this.add(boxV);

		// Schedule a job for the event-dispatching thread:
		// creating and showing this application's GUI.
		/*
		 * javax.swing.SwingUtilities.invokeLater(new Runnable() { public void
		 * run() { createAndShowGUI(); } });
		 */

		// createAndShowGUI();
		this.setOpaque(true);
	}

	public void changeOnJTable() {
		myTableModel = new MyTableModel();
		table = new JTable(myTableModel);

		table.getModel().addTableModelListener(this);

		table.setPreferredScrollableViewportSize(new Dimension(500, 70));
		table.setFillsViewportHeight(true);

		// Create the scroll pane and add the table to it.

		scrollPane = new JScrollPane(table);

		Border etched = BorderFactory.createEtchedBorder();
		scrollPane.setBorder(BorderFactory.createTitledBorder(etched,
				"Policy List"));
		// Set up column sizes.
		initColumnSizes(table);

		// Fiddle with the Sport column's cell editors/renderers.

		setUpTypeColumn(table, table.getColumnModel().getColumn(1));

		setUpContextColumn(table, table.getColumnModel().getColumn(3));
		setUpRoleColumn(table, table.getColumnModel().getColumn(4));
		setUpViewColumn(table, table.getColumnModel().getColumn(5));
		setUpActivityColumn(table, table.getColumnModel().getColumn(6));
		//setUpMeasureColumn(table, table.getColumnModel().getColumn(7));
	}

	/*
	 * This method picks good column sizes. If all column heads are wider than
	 * the column's cells' contents, then you can just use
	 * column.sizeWidthToFit().
	 */
	private void initColumnSizes(JTable table) {
		MyTableModel model = (MyTableModel) table.getModel();
		TableColumn column = null;
		Component comp = null;
		int headerWidth = 0;
		int cellWidth = 0;
		Object[] longValues = model.longValues;
		TableCellRenderer headerRenderer = table.getTableHeader()
				.getDefaultRenderer();

		for (int i = 0; i < 9; i++) {
			column = table.getColumnModel().getColumn(i);

			comp = headerRenderer.getTableCellRendererComponent(null, column
					.getHeaderValue(), false, false, 0, 0);
			headerWidth = comp.getPreferredSize().width;

			comp = table.getDefaultRenderer(model.getColumnClass(i))
					.getTableCellRendererComponent(table, longValues[i], false,
							false, 0, i);
			cellWidth = comp.getPreferredSize().width;

			if (DEBUG) {
				System.out.println("Initializing width of column " + i + ". "
						+ "headerWidth = " + headerWidth + "; cellWidth = "
						+ cellWidth);
			}

			column.setPreferredWidth(Math.max(headerWidth, cellWidth));
		}
	}

	public void setUpTypeColumn(JTable table, TableColumn column) {
		// Set up the editor for the role cells.
		JComboBox comboBox = new JComboBox();

		comboBox.addItem("protect");
		comboBox.addItem("detect");
		comboBox.addItem("response");

		column.setCellEditor(new DefaultCellEditor(comboBox));

		// Set up tool tips for the role cells.
		DefaultTableCellRenderer renderer = new DefaultTableCellRenderer();
		renderer.setToolTipText("Click for combo box");
		column.setCellRenderer(renderer);

	}

	public void setUpContextColumn(JTable table, TableColumn column) {
		// Set up the editor for the role cells.
		JComboBox comboBox = new JComboBox();

		List<String> contextList = null;
		contextList = policyDAO.getSemanticContext();
		for (int k = 0; k < contextList.size(); k++) {
			comboBox.addItem(contextList.get(k));
		}

		column.setCellEditor(new DefaultCellEditor(comboBox));

		// Set up tool tips for the role cells.
		DefaultTableCellRenderer renderer = new DefaultTableCellRenderer();
		renderer.setToolTipText("Click for combo box");
		column.setCellRenderer(renderer);

	}

	public void setUpRoleColumn(JTable table, TableColumn column) {
		// Set up the editor for the role cells.
		JComboBox comboBox = new JComboBox();

		List<String> roleList = null;
		roleList = policyDAO.getSemanticRole();
		for (int k = 0; k < roleList.size(); k++) {
			comboBox.addItem(roleList.get(k));
		}

		column.setCellEditor(new DefaultCellEditor(comboBox));

		// Set up tool tips for the role cells.
		DefaultTableCellRenderer renderer = new DefaultTableCellRenderer();
		renderer.setToolTipText("Click for combo box");
		column.setCellRenderer(renderer);
	}

	public void setUpViewColumn(JTable table, TableColumn column) {
		// Set up the editor for the role cells.
		JComboBox comboBox = new JComboBox();

		List<String> roleList = null;
		roleList = policyDAO.getSemanticView();
		for (int k = 0; k < roleList.size(); k++) {
			comboBox.addItem(roleList.get(k));
		}

		column.setCellEditor(new DefaultCellEditor(comboBox));

		// Set up tool tips for the role cells.
		DefaultTableCellRenderer renderer = new DefaultTableCellRenderer();
		renderer.setToolTipText("Click for combo box");
		column.setCellRenderer(renderer);
	}

	public void setUpActivityColumn(JTable table, TableColumn column) {
		// Set up the editor for the role cells.
		JComboBox comboBox = new JComboBox();

		List<String> roleList = null;
		roleList = policyDAO.getSemanticActivity();
		for (int k = 0; k < roleList.size(); k++) {
			comboBox.addItem(roleList.get(k));
		}

		column.setCellEditor(new DefaultCellEditor(comboBox));

		// Set up tool tips for the role cells.
		DefaultTableCellRenderer renderer = new DefaultTableCellRenderer();
		renderer.setToolTipText("Click for combo box");
		column.setCellRenderer(renderer);
	}

/*	public void setUpMeasureColumn(JTable table, TableColumn column) {
		// Set up the editor for the role cells.
		JComboBox comboBox = new JComboBox();

		List<String> roleList = null;
		roleList = policyDAO.getSemanticMeasure();
		for (int k = 0; k < roleList.size(); k++) {
			comboBox.addItem(roleList.get(k));
		}

		column.setCellEditor(new DefaultCellEditor(comboBox));

		// Set up tool tips for the role cells.
		DefaultTableCellRenderer renderer = new DefaultTableCellRenderer();
		renderer.setToolTipText("Click for combo box");
		column.setCellRenderer(renderer);
	}*/

	class MyTableModel extends AbstractTableModel {
		private String[] columnNames = { "PolicyID", "Type", "Organization",
				"Context", "Role", "View", "Activity", "Measure",
				"Choose to Update" };
		private Object[][] data;

		public MyTableModel() {
			getPolicyListData();
		}

		public void getPolicyListData() {
			int policyAmount = policyDAO.getPolicyAmount();

			data = new Object[policyAmount][columnNames.length];
			for (int i = 0; i < policyAmount; i++) {
				for (int j = 0; j < columnNames.length; j++) {
					CndPolicy cndPolicy = policyDAO.getOnePolicy(i + 1);
					if (j == 0) {
						data[i][j] = cndPolicy.getPolicyId();
					}
					if (j == 1) {
						if (0 == cndPolicy.getType()) {
							data[i][j] = "protect";
						}
						if (1 == cndPolicy.getType()) {
							data[i][j] = "detect";
						}
						if (2 == cndPolicy.getType()) {
							data[i][j] = "response";
						}
					}
					if (j == 2) {
						data[i][j] = "network";
					}
					if (j == 3) {
						data[i][j] = cndPolicy.getContext();
					}
					if (j == 4) {
						data[i][j] = cndPolicy.getRole();
					}
					if (j == 5) {
						data[i][j] = cndPolicy.getView();
					}
					if (j == 6) {
						data[i][j] = cndPolicy.getActivity();
					}
					if (j == 7) {
						data[i][j] = cndPolicy.getMeasure();
					}
					if (j == 8) {
						data[i][j] = new Boolean(false);
					}
				}
			}

		}

		public final Object[] longValues = { "XXX", "response", "network",
				"ConScopeX", "Operation_Engineer", "ViewResource",
				"ProtocolService", "detect_ICMPFlood", Boolean.TRUE };

		public int getColumnCount() {
			return columnNames.length;
		}

		public int getRowCount() {
			return data.length;
		}

		public String getColumnName(int col) {
			return columnNames[col];
		}

		public Object getValueAt(int row, int col) {
			return data[row][col];
		}

		/*
		 * JTable uses this method to determine the default renderer/ editor for
		 * each cell. If we didn't implement this method, then the last column
		 * would contain text ("true"/"false"), rather than a check box.
		 */
		public Class getColumnClass(int c) {
			return getValueAt(0, c).getClass();
		}

		/*
		 * Don't need to implement this method unless your table's editable.
		 */
		public boolean isCellEditable(int row, int col) {
			// Note that the data/cell address is constant,
			// no matter where the cell appears onscreen.
			if (col < 1) {
				return false;
			} else {
				return true;
			}
		}

		/*
		 * Don't need to implement this method unless your table's data can
		 * change.
		 */
		public void setValueAt(Object value, int row, int col) {
			if (DEBUG) {
				System.out.println("Setting value at " + row + "," + col
						+ " to " + value + " (an instance of "
						+ value.getClass() + ")");
			}

			data[row][col] = value;
			fireTableCellUpdated(row, col);

			if (DEBUG) {
				System.out.println("New value of data:");
				printDebugData();
			}
		}

		private void printDebugData() {
			int numRows = getRowCount();
			int numCols = getColumnCount();

			for (int i = 0; i < numRows; i++) {
				System.out.print("    row " + i + ":");
				for (int j = 0; j < numCols; j++) {
					System.out.print("  " + data[i][j]);
				}
				System.out.println();
			}
			System.out.println("--------------------------");
		}
	}

	/**
	 * Create the GUI and show it. For thread safety, this method should be
	 * invoked from the event-dispatching thread.
	 */
	private static void createAndShowGUI() {
		// Create and set up the window.
		// JFrame frame = new JFrame("TableRenderDemo");
		// frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

		// Create and set up the content pane.
		// UpdatePanel newContentPane = new UpdatePanel();
		// newContentPane.setOpaque(true); // content panes must be opaque

	}

	public void actionPerformed(ActionEvent e) {
		if (e.getSource() == buttonUpdate) {
			int flag = 0;
			for (int i = 0; i < myTableModel.getRowCount(); i++) {
				if ("true" == myTableModel.getValueAt(i, 8).toString()) {
					CndPolicy cndPolicy = new CndPolicy();

					try {
						cndPolicy.setPolicyId(i + 1);

						if ("protect" == myTableModel.getValueAt(i, 1)
								.toString()) {
							cndPolicy.setType(0);
						}
						if ("detect" == myTableModel.getValueAt(i, 1)
								.toString()) {
							cndPolicy.setType(1);
						}
						if ("response" == myTableModel.getValueAt(i, 1)
								.toString()) {
							cndPolicy.setType(2);
						}

						cndPolicy.setContext(myTableModel.getValueAt(i, 3)
								.toString());
						cndPolicy.setRole(myTableModel.getValueAt(i, 4)
								.toString());
						cndPolicy.setView(myTableModel.getValueAt(i, 5)
								.toString());
						cndPolicy.setActivity(myTableModel.getValueAt(i, 6)
								.toString());
						cndPolicy.setMeasure(myTableModel.getValueAt(i, 7)
								.toString());
					} catch (Exception e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}

					flag = new PolicyDAO().updatePolicy(cndPolicy);
					if (flag == 1) {
						JOptionPane.showMessageDialog(this, "策略 " + (i + 1)
								+ " Update success !", "更新成功提示框",
								JOptionPane.INFORMATION_MESSAGE);
						// 更新成功弹出对话框
					}
					// System.out.println("just check the number of row "+i);
				}
			}
			if (flag == 0) {
				JOptionPane.showMessageDialog(this,
						"Please choose the policy you want to update !",
						"用户操作提示框", JOptionPane.INFORMATION_MESSAGE);
				// 用户没有选择策略就点击更新
			}
		}
		/*
		 * if(e.getSource() == buttonRefresh){ changeOnJTable(); validate(); }
		 */
	}

	public void tableChanged(TableModelEvent e) {
		int row = e.getFirstRow();
		int column = e.getColumn();
		MyTableModel model = (MyTableModel) e.getSource();
		String columnName = model.getColumnName(column);
		Object data = model.getValueAt(row, column);

		if (1 == column) { // 如果是类型那一列发生了改动
			if ("protect" == data) {
				setUpProtectMeasureColumn(table, table.getColumnModel().getColumn(7));
				//model.getValueAt(row, 7);
			}
			if ("detect" == data) {
				setUpDetectMeasureColumn(table, table.getColumnModel().getColumn(7));
			}
			if ("response" == data) {
				setUpResponseMeasureColumn(table, table.getColumnModel().getColumn(7));
			}
		}

	}

	private void setUpResponseMeasureColumn(JTable table2, TableColumn column) {
		JComboBox comboBox = new JComboBox();

		comboBox.addItem("stop_service");
		
    	column.setCellEditor(new DefaultCellEditor(comboBox));

		// Set up tool tips for the role cells.
		DefaultTableCellRenderer renderer = new DefaultTableCellRenderer();
		renderer.setToolTipText("Click for combo box");
		column.setCellRenderer(renderer);
		
	}

	private void setUpDetectMeasureColumn(JTable table2, TableColumn column) {
		JComboBox comboBox = new JComboBox();

		comboBox.addItem("detect_ICMPFlood");
		comboBox.addItem("detect_SYNFlood");
		comboBox.addItem("detect_IPSpoof");

		column.setCellEditor(new DefaultCellEditor(comboBox));

		// Set up tool tips for the role cells.
		DefaultTableCellRenderer renderer = new DefaultTableCellRenderer();
		renderer.setToolTipText("Click for combo box");
		column.setCellRenderer(renderer);
		
	}

	private void setUpProtectMeasureColumn(JTable table2, TableColumn column) {
		JComboBox comboBox = new JComboBox();

		comboBox.addItem("permit");
		comboBox.addItem("deny");
		
		column.setCellEditor(new DefaultCellEditor(comboBox));

		// Set up tool tips for the role cells.
		DefaultTableCellRenderer renderer = new DefaultTableCellRenderer();
		renderer.setToolTipText("Click for combo box");
		column.setCellRenderer(renderer);
		
	}
}

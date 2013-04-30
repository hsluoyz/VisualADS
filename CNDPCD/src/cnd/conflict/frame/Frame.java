package cnd.conflict.frame;

import java.awt.Container;
import java.awt.Dimension;
import java.awt.MenuBar;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.Box;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTextArea;
import javax.swing.JTree;
import javax.swing.border.Border;
import javax.swing.tree.DefaultMutableTreeNode;

import cnd.conflict.dao.PolicyDAO;
import cnd.conflict.service.Service;
import cnd.conflict.util.DataBaseConn;

/**
 * 主界面对象
 * 
 */
public class Frame extends JFrame {

	//private java.util.ResourceBundle resourceBundle;
	private int chineseOrEnglish = 1;
	
	private int width = 800;
	private int height = 600;
	private int verticalHeight = 15;
	private int horizontalWidth = 15;
	private JTextArea messageArea;
	private JTable table;
	private JTree tree = createTree();
	

	public JTextArea getMessageArea() {
		return messageArea;
	}
 
	private JScrollPane scrollPaneTree;
	private JScrollPane scrollPaneMessage;
	private JScrollPane scrollPaneTable;
	private Box boxV, boxH, boxHTable;

	// JLabel label = new JLabel();
	// JTextPane textPane = new JTextPane();
	private Container con = getContentPane();

	private Service service = Service.getServiceInstance(this);
	 
	private DataBaseConn dataBaseConn =  DataBaseConn.getDataBaseConnInstance();
	
	private PolicyDAO policyDAO = new PolicyDAO();

	// 加给菜单的事件监听器
	ActionListener menuListener = new ActionListener() {
		public void actionPerformed(ActionEvent e) {
			service.menuDo(Frame.this, e.getActionCommand());
		}
	};

	/**
	 * 构造器
	 */
	public Frame() {
		super();
		// 初始化这个JFrame
		init();
	}

	/**
	 * 初始化
	 * 
	 * @return void
	 */
	public void init() {
		// 设置标题
		this.setTitle("计算机网络防御策略冲突检测工具");
		// 设置大小
		this.setPreferredSize(new Dimension(width, height));
		// 创建菜单
		createMenuBar();
		// 创建工具栏
		
		// 创建国际化字符串资源
		//resourceBundle = java.util.ResourceBundle.getBundle("langue/softwareResources", java.util.Locale.CHINESE);
		// 设置语言模式为中文
		setLanguage(1);

		// createTree();
		messageArea = createMessage();
		// createTable();

		// 布局
		boxH = Box.createHorizontalBox();
		boxV = Box.createVerticalBox();
		boxHTable = Box.createHorizontalBox();

		// scrollPaneTree = new JScrollPane(createTreeTest());
		scrollPaneTree = new JScrollPane(createTree());
		Border etched = BorderFactory.createEtchedBorder();
		scrollPaneTree.setBorder(BorderFactory.createTitledBorder(etched,
				"Policy Tree"));
		boxH.add(Box.createHorizontalStrut(horizontalWidth));
		boxH.add(scrollPaneTree);
		boxH.add(Box.createHorizontalStrut(horizontalWidth));
		scrollPaneMessage = new JScrollPane(messageArea);
		scrollPaneMessage.setBorder(BorderFactory.createTitledBorder(etched,
				"Messages"));
		boxH.add(scrollPaneMessage);
		boxH.add(Box.createHorizontalStrut(horizontalWidth));

		scrollPaneTable = new JScrollPane(createTable());
		scrollPaneTable.setBorder(BorderFactory.createTitledBorder(etched,
				"Policy List"));
		boxHTable.add(Box.createHorizontalStrut(horizontalWidth));
		boxHTable.add(scrollPaneTable);
		boxHTable.add(Box.createHorizontalStrut(horizontalWidth));

		boxV.add(Box.createVerticalStrut(verticalHeight));
		boxV.add(boxH);
		boxV.add(Box.createVerticalStrut(verticalHeight));
		boxV.add(boxHTable);
		boxV.add(Box.createVerticalStrut(verticalHeight));

		this.add(boxV);
		setUndecorated(true);
		this.setVisible(true);
		validate();
		this.pack();
	}

	/**
	 * 创建菜单栏
	 * 
	 * @return void
	 */
	public void createMenuBar() {
		// 创建一个JMenuBar放置菜单
		JMenuBar menuBar = new JMenuBar();

		// 菜单文字数组，以下面的menuItemArr一一对应
		// String[] menuArr = { "文件(F)", "工具(T)", "结果显示(R)","帮助(H)" };
		String[] menuArr = { "文件", "切换检测策略类型","策略预处理", "语义建模", "冲突检测", "结果分析", "切换语言", "帮助" };
		// 菜单项文字数组
		String[][] menuItemArr = {
				{ "打开(O)", "-", "保存(S)", "-", "编辑(E)", "-", "退出(X)" },
				{"连接样例数据库","-", "检测文件访问控制策略"},
				{ "策略解析", "-", "语义映射" , "-", "策略列表"},
				{ "本体导入", "-", "实例加载", "-", "关系图生成" },
				{ "冲突检测", "-", "生成报告" }, { "检测结果分析" }, { "英文", "-", "中文" },
				{ "帮助", "-", "关于" } };

		// String[][] menuItemArr = { { "Open(O)", "-", "Exit(X)" },
		// { "放大(M)", "缩小(O)", "-", "上一个(X)", "下一个(P)" }, {"结果显示"},{ "帮助主题",
		// "关于" } };
		// 遍历menuArr与menuItemArr去创建菜单
		for (int i = 0; i < menuArr.length; i++) {
			// 新建一个JMenu菜单
			JMenu menu = new JMenu(menuArr[i]);
			for (int j = 0; j < menuItemArr[i].length; j++) {
				// 如果menuItemArr[i][j]等于"-"
				if (menuItemArr[i][j].equals("-")) {
					// 设置菜单分隔
					menu.addSeparator();
				} else {
					// 新建一个JMenuItem菜单项
					JMenuItem menuItem = new JMenuItem(menuItemArr[i][j]);
					menuItem.addActionListener(menuListener);
					// 把菜单项加到JMenu菜单里面
					menu.add(menuItem);
				}
			}
			// 把菜单加到JMenuBar上
			menuBar.add(menu);
		}
		// 设置JMenubar
		this.setJMenuBar(menuBar);
	}
	
	public void updateMenuBarToEnglish() {
		setLanguage(0);
		JMenuBar menuBar = this.getJMenuBar();
		menuBar.removeAll();
		
		// 菜单文字数组，以下面的menuItemArr一一对应
		// String[] menuArr = { "文件(F)", "工具(T)", "结果显示(R)","帮助(H)" };
		String[] menuArr = { "File", "Mode","Preprocess", "Semantic-Model", "Detect", "Result", "Language", "Help" };
		// 菜单项文字数组
		String[][] menuItemArr = {
				{ "Open(O)", "-", "Save(S)", "-", "Edit(E)", "-", "Exit(X)" },
				{"Connect Sample Database","-", "Detect File Policies"},
				{ "Parse Policies", "-", "Semantic Mapping" , "-", "Policy List"},
				{ "Import Ontology", "-", "Load Instance", "-", "Generate Relation Graph" },
				{ "Detect Conflicts", "-", "Generate Report" }, { "Analyze Result" }, { "English", "-", "Chinese" }, 
				{ "Help", "-", "About" } };
		
		// 遍历menuArr与menuItemArr去创建菜单
		for (int i = 0; i < menuArr.length; i++) {
			// 新建一个JMenu菜单
			JMenu menu = new JMenu(menuArr[i]);
			for (int j = 0; j < menuItemArr[i].length; j++) {
				// 如果menuItemArr[i][j]等于"-"
				if (menuItemArr[i][j].equals("-")) {
					// 设置菜单分隔
					menu.addSeparator();
				} else {
					// 新建一个JMenuItem菜单项
					JMenuItem menuItem = new JMenuItem(menuItemArr[i][j]);
					menuItem.addActionListener(menuListener);
					// 把菜单项加到JMenu菜单里面
					menu.add(menuItem);
				}
			}
			// 把菜单加到JMenuBar上
			menuBar.add(menu);
		}
		// 设置JMenubar
		this.setJMenuBar(menuBar);
	}
	
	public void updateMenuBarToChinese() {
		setLanguage(1);
		JMenuBar menuBar = this.getJMenuBar();
		menuBar.removeAll();
		
		// 菜单文字数组，以下面的menuItemArr一一对应
		// String[] menuArr = { "文件(F)", "工具(T)", "结果显示(R)","帮助(H)" };
		String[] menuArr = { "文件", "切换检测策略类型","策略预处理", "语义建模", "冲突检测", "结果分析", "切换语言", "帮助" };
		// 菜单项文字数组
		String[][] menuItemArr = {
				{ "打开(O)", "-", "保存(S)", "-", "编辑(E)", "-", "退出(X)" },
				{"连接样例数据库","-", "检测文件访问控制策略"},
				{ "策略解析", "-", "语义映射" , "-", "策略列表"},
				{ "本体导入", "-", "实例加载", "-", "关系图生成" },
				{ "冲突检测", "-", "生成报告" }, { "检测结果分析" }, { "英文", "-", "中文" },
				{ "帮助", "-", "关于" } };
		
		// 遍历menuArr与menuItemArr去创建菜单
		for (int i = 0; i < menuArr.length; i++) {
			// 新建一个JMenu菜单
			JMenu menu = new JMenu(menuArr[i]);
			for (int j = 0; j < menuItemArr[i].length; j++) {
				// 如果menuItemArr[i][j]等于"-"
				if (menuItemArr[i][j].equals("-")) {
					// 设置菜单分隔
					menu.addSeparator();
				} else {
					// 新建一个JMenuItem菜单项
					JMenuItem menuItem = new JMenuItem(menuItemArr[i][j]);
					menuItem.addActionListener(menuListener);
					// 把菜单项加到JMenu菜单里面
					menu.add(menuItem);
				}
			}
			// 把菜单加到JMenuBar上
			menuBar.add(menu);
		}
		// 设置JMenubar
		this.setJMenuBar(menuBar);
	}

	private JTree createTree() {
		DefaultMutableTreeNode root = new DefaultMutableTreeNode("CNDPOLICY");
		tree = new JTree(root);
		return tree;
	}

	private JTextArea createMessage() {
		JTextArea messageArea = new JTextArea(6, 20);
		return messageArea;
	}

	public JTable createTable() {

		Object a[][];
		Object name[] = { "PolicyID", "Organization", "Context", "Role",
				"View", "Activity", "Measure" };
		// 填写表中的内容

		int policyAmount = policyDAO.getPolicyAmount();

		a = new Object[policyAmount][name.length];

		table = new JTable(a, name);
		return table;
	}

	public JTable getJTable() {
		return this.table;
	}

	public JTree getJTree() {
		return this.tree;
	}

	public int getLanguage() {
		return chineseOrEnglish;
	}
	
	public void setLanguage(int chineseOrEnglish) {
		this.chineseOrEnglish = chineseOrEnglish;
	}
}
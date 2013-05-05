package cnd.conflict.service;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.Enumeration;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Set;
import java.util.StringTokenizer;

import javax.swing.JFileChooser;
import javax.swing.JLabel;
import javax.swing.JOptionPane;
import javax.swing.JTable;
import javax.swing.JTree;
import javax.swing.SwingConstants;
import javax.swing.table.DefaultTableCellRenderer;
import javax.swing.table.DefaultTableModel;
import javax.swing.table.JTableHeader;
import javax.swing.table.TableColumn;
import javax.swing.tree.DefaultMutableTreeNode;
import javax.swing.tree.DefaultTreeModel;

import cnd.conflict.dao.PolicyDAO;
import cnd.conflict.detect.ConflictAnalysis;
import cnd.conflict.detect.RelationGraph;
import cnd.conflict.detect.Report;
import cnd.conflict.entity.CndPolicy;
import cnd.conflict.frame.Cndpcd;
import cnd.conflict.frame.Frame;
import cnd.conflict.frame.PolicyEditor;
import cnd.conflict.frame.ResultAnalysisFrame;
import cnd.conflict.util.About;
import cnd.conflict.util.DataBaseConn;
import cnd.conflict.util.Help;
import cnd.conflict.util.ResultBarChart;

/**
 * 业务类
 * 
 * @author YC LYZ
 */

public class Service {
	private static Service service = null;
	private static Frame frame;

	private DataBaseConn dataBaseConn = DataBaseConn.getDataBaseConnInstance();

	// JFileChooser
	private JFileChooser filechooser = new JFileChooser();

	private RelationGraph relationGraph = RelationGraph.getInstance(frame);

	private ConflictAnalysis conflictAnalysis = ConflictAnalysis.getInstance();

	PolicyDAO policyDAO = new PolicyDAO();
	private PolicyEditor policyEditor;

	private Report notepadSample = Report.getInstance();

	private List<String> contentArr = new ArrayList<String>();

	// 类中需要定义的域中信息
	private String[] domainNames = new String[20];

	
	public static Frame getFrame() {
		return frame;
	}

	public PolicyDAO getPolicyDAO() {
		return policyDAO;
	}

	public String[] getDomainNames() {
		return domainNames;
	}

	/**
	 * 私有构造器
	 */
	protected Service(Frame frame) {
		Service.frame = frame;
	}

	/**
	 * 获取单态实例
	 * 
	 * @return Service
	 */
	public static Service getServiceInstance(Frame frame) {
		Service.frame = frame;
		if (service == null) {
			service = new Service(frame);
		}

		return service;
	}

	/**
	 * 打开文件
	 * 
	 * @param frame
	 * 
	 * @return void
	 */
	public void open(Frame frame) {
		// 如果选择打开
		int i = filechooser.showOpenDialog(frame); // 显示打开文件对话框

		if (i == JFileChooser.APPROVE_OPTION) { // 点击对话框中打开选项
			File f = filechooser.getSelectedFile(); // 得到选择的文件
			try {
				BufferedReader br = new BufferedReader(new FileReader(f));
				String str;

				StringBuffer strBuffer = new StringBuffer();
				while ((str = br.readLine()) != null) {
					contentArr.add(str);
					strBuffer.append(str + "\r\n");
				}

				frame.getMessageArea().setText(strBuffer.toString());

				if (br != null) {
					br.close();
				}
			} catch (Exception ex) {
				ex.printStackTrace(); // 输出出错信息
			}
		}

	}

	// 策略解析时将文件内容信息存入数据库
	public void policyAnalysis(Frame frame) {
		String analysisInfo;
		if ("fileservices" == dataBaseConn.getUrl_database()) {
			//开启文件读取线程
			new FileReadThread(this).start();
		}

	}

	public String initPolicyDir() {
		String curDir = null;
		String filePath = System.getProperty("user.dir");
		if (isFileExist(filePath + "/resources")) {
			curDir = filePath + "/resources/";
		} else {
			// 获得JAR包所在路径
			filePath = new Cndpcd().getClass().getProtectionDomain()
					.getCodeSource().getLocation().getFile();
			try {
				filePath = java.net.URLDecoder.decode(filePath, "UTF-8");
			} catch (java.io.UnsupportedEncodingException e) {
				e.printStackTrace();
			}
			java.io.File jarFile = new java.io.File(filePath);
			java.io.File parent = jarFile.getParentFile();
			java.io.File grandPa = parent.getParentFile();
			if (grandPa != null) {
				filePath = grandPa.getAbsolutePath();
				try {
					filePath = java.net.URLDecoder.decode(filePath, "UTF-8");
				} catch (java.io.UnsupportedEncodingException e) {
					e.printStackTrace();
				}
			}

			if (isFileExist(filePath + "/policy")) {
				curDir = filePath + "/policy/";
			} else {
				JOptionPane.showMessageDialog(null, "initPolicyDir Error",
						"Error", JOptionPane.ERROR_MESSAGE);
			}
		}
		return curDir;
	}

	private boolean isFileExist(String filePath) {
		File f = new File(filePath);
		if (f.exists()) {
			return true;
		} else {
			return false;
		}
	}

	public void initJTable(Frame frame) {
		JTable table = frame.getJTable();
		Object a[][];
		Object name[] = { "PolicyID", "Organization", "Context", "Role",
				"View", "Activity", "Measure" };
		int policyAmount = policyDAO.getPolicyAmount();

		a = new Object[policyAmount][name.length];
		for (int i = 0; i < policyAmount; i++) {
			for (int j = 0; j < name.length; j++) {
				CndPolicy cndPolicy = policyDAO.getOnePolicy(i + 1);
				if (j == 0) {
					a[i][j] = cndPolicy.getPolicyId();
				}
				if (j == 1) {
					a[i][j] = cndPolicy.getOrganization();
				}
				if (j == 2) {
					a[i][j] = cndPolicy.getContext();
				}
				if (j == 3) {
					a[i][j] = cndPolicy.getRole();
				}
				if (j == 4) {
					a[i][j] = cndPolicy.getView();
				}
				if (j == 5) {
					a[i][j] = cndPolicy.getActivity();
				}
				if (j == 6) {
					a[i][j] = cndPolicy.getMeasure();
				}
			}
		}
		DefaultTableModel model = new DefaultTableModel(a, name);
		table.setModel(model);

		table.setAutoResizeMode(JTable.AUTO_RESIZE_OFF);

		FitTableColumns(table);

		DefaultTableCellRenderer r = new DefaultTableCellRenderer();
		r.setHorizontalAlignment(JLabel.CENTER);
		table.setDefaultRenderer(Object.class, r);
	}

	public void FitTableColumns(JTable myTable) {
		JTableHeader header = myTable.getTableHeader();
		int rowCount = myTable.getRowCount();
		Enumeration columns = myTable.getColumnModel().getColumns();
		while (columns.hasMoreElements()) {
			TableColumn column = (TableColumn) columns.nextElement();
			int col = header.getColumnModel().getColumnIndex(
					column.getIdentifier());
			int width = (int) myTable.getTableHeader().getDefaultRenderer()
					.getTableCellRendererComponent(myTable,
							column.getIdentifier(), false, false, -1, col)
					.getPreferredSize().getWidth();
			for (int row = 0; row < rowCount; row++) {
				int preferedWidth = (int) myTable.getCellRenderer(row, col)
						.getTableCellRendererComponent(myTable,
								myTable.getValueAt(row, col), false, false,
								row, col).getPreferredSize().getWidth();
				width = Math.max(width, preferedWidth);
			}
			header.setResizingColumn(column); // 此行很重要
			// +10留有一定空隙
			column.setWidth(width + myTable.getIntercellSpacing().width + 10);
		}
	}

	public void initJTree(Frame frame) {
		JTree tree = frame.getJTree();
		DefaultMutableTreeNode root = new DefaultMutableTreeNode("CNDPOLICY");
		DefaultMutableTreeNode t1 = new DefaultMutableTreeNode("Organization");
		DefaultMutableTreeNode t1_1 = new DefaultMutableTreeNode("network");
		DefaultMutableTreeNode t2 = new DefaultMutableTreeNode("Context");

		DefaultMutableTreeNode t3 = new DefaultMutableTreeNode("Role");

		DefaultMutableTreeNode t4 = new DefaultMutableTreeNode("View");

		DefaultMutableTreeNode t5 = new DefaultMutableTreeNode("Activity");

		DefaultMutableTreeNode t6 = new DefaultMutableTreeNode("Type");

		DefaultMutableTreeNode t6_1 = new DefaultMutableTreeNode("Protect");
		DefaultMutableTreeNode t6_2 = new DefaultMutableTreeNode("Detect");
		DefaultMutableTreeNode t6_3 = new DefaultMutableTreeNode("Response");

		DefaultMutableTreeNode t7 = new DefaultMutableTreeNode("Measure");

		root.add(t1);
		t1.add(t1_1);
		root.add(t2);
		root.add(t3);
		root.add(t4);
		root.add(t5);
		root.add(t6);
		t6.add(t6_1);
		t6.add(t6_2);
		t6.add(t6_3);
		root.add(t7);
		DefaultTreeModel model = new DefaultTreeModel(root);
		tree.setModel(model);
		try {
			BufferedReader br = new BufferedReader(new FileReader(new File(
					"K:/javatest/CNDPCD-20130314/resources/CNDPolicy.owl")));
			// akisn0w
			/*
			 * BufferedReader br = new BufferedReader(new FileReader(new File(
			 * "C:/Users/Administrator/Desktop/VisualADS/CNDPCD/resources/CNDPolicy.owl"
			 * )));
			 */
			String str = null;
			StringBuffer strBuffer = new StringBuffer();
			try {
				while ((str = br.readLine()) != null) {
					strBuffer.append(str);
					strBuffer.append("\n");
				}
				frame.getMessageArea().setText(strBuffer.toString());
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		} catch (FileNotFoundException e) {

			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

	public void buildJTree(Frame frame) {
		JTree tree = frame.getJTree();
		int policyAmount = policyDAO.getPolicyAmount();
		HashMap<Integer, String> eMap = null;
		DefaultMutableTreeNode root = new DefaultMutableTreeNode("CNDPOLICY");
		DefaultMutableTreeNode t1 = new DefaultMutableTreeNode("Organization");

		DefaultMutableTreeNode t6_1 = null;
		DefaultMutableTreeNode t6_2 = null;
		DefaultMutableTreeNode t6_3 = null;

		DefaultMutableTreeNode t6 = new DefaultMutableTreeNode("Type");

		if ("fileservices" == dataBaseConn.getUrl_database()) {
			DefaultMutableTreeNode[] t1_1 = new DefaultMutableTreeNode[this.domainNames.length];
			for (int k = 0; k < this.domainNames.length; k++) {
				if (null != this.domainNames[k]) {
					t1_1[k] = new DefaultMutableTreeNode(this.domainNames[k]);
					t1.add(t1_1[k]);
				}
			}
			// t1_1 = new DefaultMutableTreeNode("");
			t6_1 = new DefaultMutableTreeNode("Protect");
		} else {
			DefaultMutableTreeNode t1_1 = new DefaultMutableTreeNode("network");
			t6_1 = new DefaultMutableTreeNode("Protect");
			t6_2 = new DefaultMutableTreeNode("Detect");
			t6_3 = new DefaultMutableTreeNode("Response");
			t1.add(t1_1);
			t6.add(t6_2);
			t6.add(t6_3);
		}
		DefaultMutableTreeNode t2 = new DefaultMutableTreeNode("Context");

		eMap = policyDAO.getElementMap(3);
		for (int i = 1; i <= policyAmount; i++) {
			String str = "policy" + i + "_" + eMap.get(i);
			DefaultMutableTreeNode t2_i = new DefaultMutableTreeNode(str);
			t2.add(t2_i);
		}
		DefaultMutableTreeNode t3 = new DefaultMutableTreeNode("Role");
		eMap = policyDAO.getElementMap(4);
		for (int i = 1; i <= policyAmount; i++) {
			String str = "policy" + i + "_" + eMap.get(i);
			DefaultMutableTreeNode t3_i = new DefaultMutableTreeNode(str);
			t3.add(t3_i);
		}
		DefaultMutableTreeNode t4 = new DefaultMutableTreeNode("View");
		eMap = policyDAO.getElementMap(5);
		for (int i = 1; i <= policyAmount; i++) {
			String str = "policy" + i + "_" + eMap.get(i);
			DefaultMutableTreeNode t4_i = new DefaultMutableTreeNode(str);
			t4.add(t4_i);
		}
		DefaultMutableTreeNode t5 = new DefaultMutableTreeNode("Activity");
		eMap = policyDAO.getElementMap(6);
		for (int i = 1; i <= policyAmount; i++) {
			String str = "policy" + i + "_" + eMap.get(i);
			DefaultMutableTreeNode t5_i = new DefaultMutableTreeNode(str);
			t5.add(t5_i);
		}

		HashMap<Integer, Integer> tMap = new HashMap<Integer, Integer>();
		tMap = policyDAO.getTypeMap(2);
		for (int i = 1; i <= policyAmount; i++) {
			if (tMap.get(i) == 0) {
				DefaultMutableTreeNode t6__ = new DefaultMutableTreeNode(
						"policy" + i);
				t6_1.add(t6__);
			}
			if (tMap.get(i) == 1) {
				DefaultMutableTreeNode t6__ = new DefaultMutableTreeNode(
						"policy" + i);
				t6_2.add(t6__);
			}
			if (tMap.get(i) == 2) {
				DefaultMutableTreeNode t6__ = new DefaultMutableTreeNode(
						"policy" + i);
				t6_3.add(t6__);
			}
		}

		DefaultMutableTreeNode t7 = new DefaultMutableTreeNode("Measure");
		eMap = policyDAO.getElementMap(7);
		for (int i = 1; i <= policyAmount; i++) {
			String str = "policy" + i + "_" + eMap.get(i);
			DefaultMutableTreeNode t7_i = new DefaultMutableTreeNode(str);
			t7.add(t7_i);
		}
		root.add(t1);

		root.add(t2);
		root.add(t3);
		root.add(t4);
		root.add(t5);
		root.add(t6);
		t6.add(t6_1);

		root.add(t7);

		DefaultTreeModel model = new DefaultTreeModel(root);
		tree.setModel(model);
		frame.getMessageArea().setText("policy load success!");
	}

	/**
	 * 
	 * @param frame
	 * @return
	 */
	public void save(Frame frame) {
		int i = filechooser.showSaveDialog(frame); // 显示保存文件对话框
		if (i == JFileChooser.APPROVE_OPTION) { // 点击对话框中保存按钮
			File f = filechooser.getSelectedFile(); // 得到选择的文件
			try {
				FileOutputStream out = new FileOutputStream(f); // 得到文件输出流
				// out.write(frame.getTextPane().getText().getBytes()); //写出文件
			} catch (Exception ex) {
				ex.printStackTrace(); // 输出出错信息
			}
		}
	}

	public void report(Frame frame) {
		try {
			notepadSample.init(frame);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public void help(Frame frame) {
		try {
			new Help(frame);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public void about(Frame frame) {
		try {
			new About(frame);
		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

	public void semanticMap(Frame frame) {

		StringBuffer strBuffer = new StringBuffer();

		strBuffer.append("Role Semantic Map:");
		strBuffer.append("\n");
		for (String element : policyDAO.getSemanticRoleMap()) {
			strBuffer.append(element);
			strBuffer.append("\n");
		}
		strBuffer.append("View Semantic Map:");
		strBuffer.append("\n");
		for (String element : policyDAO.getSemanticViewMap()) {
			strBuffer.append(element);
			strBuffer.append("\n");
		}
		strBuffer.append("Activity Semantic Map:");
		strBuffer.append("\n");
		for (String element : policyDAO.getSemanticActivityMap()) {
			strBuffer.append(element);
			strBuffer.append("\n");
		}

		frame.getMessageArea().setText(strBuffer.toString());

	}

	private void result(Frame frame) {
		// TODO Auto-generated method stub
		new ResultBarChart();
		new ResultAnalysisFrame(frame);

	}

	/*
	 * public void test(Frame frame){ frame.test(); }
	 */

	/**
	 * 响应菜单的动作
	 * 
	 * @param frame
	 * 
	 * @param cmd
	 * 
	 * @return void
	 */
	public void menuDo(Frame frame, String cmd) {
		// 打开
		if (cmd.equals("打开(O)") || cmd.equals("Open(O)")) {
			open(frame);
		}
		// 保存
		if (cmd.equals("保存(S)") || cmd.equals("Save(S)")) {
			save(frame);
		}
		// 编辑
		if (cmd.equals("编辑(E)") || cmd.equals("Edit(E)")) {

			if (policyEditor == null) {
				policyEditor = PolicyEditor.getInstance();
				policyEditor.init();
			} else {
				policyEditor.setVisible(true);
			}
			/*
			 * policyEditor = PolicyEditor.getInstance(); policyEditor.init();
			 */

		}
		// 连接样例数据库
		if (cmd.equals("连接样例数据库") || cmd.equals("Connect Sample Database")) {
			dataBaseConn.closeConn();
			dataBaseConn.setUrl_database("policy");
			if (cmd.equals("Connect Sample Database")) {
				frame.getMessageArea().setText("Sample database connected");
			} else {
				frame.getMessageArea().setText("已连接到样例数据库");
			}
			System.out.println(dataBaseConn.getUrl_database());
		}
		if (cmd.equals("检测文件访问控制策略") || cmd.equals("Detect File Policies")) {
			dataBaseConn.closeConn();
			dataBaseConn.setUrl_database("fileservices");
			// 清空数据库中表的数据
			policyDAO.deleteAllTable();
			if (cmd.equals("Detect File Policies")) {
				frame.getMessageArea()
						.setText("FileService database connected");
			} else {
				frame.getMessageArea().setText("已连接到文件服务数据库");
			}
			System.out.println(dataBaseConn.getUrl_database());
		}
		// 策略解析
		if (cmd.equals("策略解析") || cmd.equals("Parse Policies")) {
			policyAnalysis(frame);
		}
		// 策略列表
		if (cmd.equals("策略列表") || cmd.equals("Policy List")) {
			initJTable(frame);
		}
		// 语义映射
		if (cmd.equals("语义映射") || cmd.equals("Semantic Mapping")) {
			// TODO
			semanticMap(frame);
		}
		// 初始本体导入
		if (cmd.equals("本体导入") || cmd.equals("Import Ontology")) {
			initJTree(frame);
		}
		// 实例加载
		if (cmd.equals("实例加载") || cmd.equals("Load Instance")) {
			buildJTree(frame);
		}
		// 关系图生成
		if (cmd.equals("关系图生成") || cmd.equals("Generate Relation Graph")) {
			relationGraph.init(frame);
		}
		// 冲突检测
		if (cmd.equals("冲突检测") || cmd.equals("Detect Conflicts")) {
			if (frame == null || frame.getLanguage() == 1) {
				if ("fileservices" == dataBaseConn.getUrl_database()) {
					conflictAnalysis.conflict_Analysis(frame, "fileservices");
				} else {
					conflictAnalysis.conflict_Analysis(frame);
				}
			} else {
				if ("fileservices" == dataBaseConn.getUrl_database()) {
					conflictAnalysis.conflict_Analysis_English(frame,
							"fileservices");
				} else {
					conflictAnalysis.conflict_Analysis_English(frame);
				}
			}
		}
		// 生成报告
		if (cmd.equals("生成报告") || cmd.equals("Generate Report")) {
			report(frame);
		}
		// 检测结果分析
		if (cmd.equals("检测结果分析") || cmd.equals("Analyze Result")) {
			result(frame);
		}
		// 英文
		if (cmd.equals("英文") || cmd.equals("English")) {
			frame.updateMenuBarToEnglish();
		}
		// 中文
		if (cmd.equals("中文") || cmd.equals("Chinese")) {
			frame.updateMenuBarToChinese();
		}
		// 帮助
		if (cmd.equals("帮助") || cmd.equals("Help")) {
			help(frame);
		}
		// 关于
		if (cmd.equals("关于") || cmd.equals("About")) {
			about(frame);
		}
		// 退出
		if (cmd.equals("退出(X)") || cmd.equals("Exit(X)")) {
			System.exit(0);
		}

	}

}

/**
 * 文件读取线程
 */
class FileReadThread implements Runnable {
	
	private Thread runner;
	private Service service;

	public FileReadThread(Service service) {
       this.service = service;
	}

	public void start() {
		// TODO Auto-generated method stub
		runner = new Thread(this);
		runner.start();
	}

	@Override
	public void run() {
		String analysisInfo = "Policy file is analyzing...";
		service.getFrame().getMessageArea().setText(analysisInfo + "\n");
		FileInputStream fis = null;
		InputStreamReader isr = null;
		// activity 表
		service.getPolicyDAO().addActivity("R", "FileServices");
		service.getPolicyDAO().addActivity("W", "FileServices");
		service.getPolicyDAO().addActivity("X", "FileServices");
		// measure表
		service.getPolicyDAO().addMeasure("permit", "deny");
		service.getPolicyDAO().addMeasure("deny", "permit");
		// semantic_activity表
		service.getPolicyDAO().addActivitySemantic("R", "Read");
		service.getPolicyDAO().addActivitySemantic("W", "Write");
		service.getPolicyDAO().addActivitySemantic("X", "Execute");
		// System.out.println("********************");
		// sematic_meature表
		service.getPolicyDAO().addMeasureSemantic("permint", "", 0);
		service.getPolicyDAO().addMeasureSemantic("deny", "", 0);
		// System.out.println("********************");
		BufferedReader br = null; // 用于包装InputStreamReader,提高处理性能。因为BufferedReader有缓冲的，而InputStreamReader没有。
		try {
			// 实现了数据库切换的话每个数据库中的表就是已经建立好的
			String str = "";
			// System.out.println("********************");
			// fis = new
			// FileInputStream("./resources/CNDP_FileService.txt");//
			// FileInputStream
			// fis = new
			// FileInputStream("K:/javatest/CNDPCD-20130314/bin/CNDP_FileService.txt");
			// akisn0w
			// fis = new
			// FileInputStream("C:/Users/Administrator/Desktop/VisualADS/policy/CNDP.txt");
			// fis = new
			// FileInputStream("C:/Users/Administrator/Desktop/VisualADS/CNDPCD/resources/CNDP_FileService.txt");
			// 从文件系统中的某个文件中获取字节
			// 如果不是第一次读取相关类型文件则单纯把数据库中的表清空即可

			// 设置相对路径 判断改软件是从哪个环境下运行的 JAR包、eclipse
			String PFilePath = service.initPolicyDir() + "CNDP_FileService.txt";
			fis = new FileInputStream(PFilePath);

			isr = new InputStreamReader(fis);// InputStreamReader
			// 是字节流通向字符流的桥梁,
			br = new BufferedReader(isr);// 从字符输入流中读取文件中的内容,封装了一个new
			// InputStreamReader的对象

			int authPFlag = 0;
			int inhePFlag = 0;

			Set<String> contextSet = new HashSet<String>();
			Set<String> viewSet = new HashSet<String>();
			Set<String> semantic_viewSet = new HashSet<String>();
			Set<String> semantic_contextSet = new HashSet<String>();

			Set<String> roleSet = new HashSet<String>();

			String domainName = null;

			int domainNum = 0;

			while ((str = br.readLine()) != null) {
				// 遇到“{”即进入一个域的相关策略处理 遇到“}”结束对这个域的处理
				// 关键字 Authorization: Inheritance: ChildGroup: ChildUser:
				// NULL Everyone 分隔号::
				// 保留域名

				if (str.endsWith("{")) {
					domainName = str.substring(0, str.length() - 1);
					analysisInfo = "Starting parsing domain: " + domainName;
					service.getFrame().getMessageArea().append(analysisInfo + "\n");
					// frame.getMessageArea().setText(analysisInfo);
					// System.out.println(domainName);
				}
				if (str.equals("}")) {
					// 一个域信息的处理结束
					// 将一个域的域名保存在数组中
					analysisInfo = "Finished parsing domain: " + domainName;
					service.getFrame().getMessageArea().append(analysisInfo + "\n");
					// frame.getMessageArea().setText(analysisInfo);
					service.getDomainNames()[domainNum] = domainName;
					domainNum++;
					domainName = null;
					authPFlag = 0;
					inhePFlag = 0;
				}

				if (inhePFlag == 1) {
					// 将继承策略插入到对应的表中
					
					String[] s = str.split(":");
					// role表两个元素
					String roleParentElement = s[0].substring(1, s[0]
							.indexOf("ChildGroup") - 1);

					if (roleSet.isEmpty()) {
						roleSet.add(domainName + "\\" + roleParentElement);
						service.getPolicyDAO().addRoleSemantic(domainName + "\\"
								+ roleParentElement, "Group");
					}
					if (!roleSet.contains(domainName + "\\"
							+ roleParentElement)) {
						roleSet.add(domainName + "\\" + roleParentElement);
						service.getPolicyDAO().addRoleSemantic(domainName + "\\"
								+ roleParentElement, "Group");
					}

					String roleElement = null;
					// semanticrole表两个元素
					// String role_RoleSemanticTable = null;
					// String semantic_RoleSemanticTable = null;
					String sChildGroup = s[1].substring(0, s[1]
							.indexOf("ChildUser"));
					String[] childGroup = sChildGroup.split(">");
					for (int i = 0; i < childGroup.length; i++) {
						System.out.println("*" + childGroup[i] + "*");

					}
					for (int i = 0; i < childGroup.length; i++) {
						if (!(childGroup[i].contains("?"))) {
							roleElement = childGroup[i].substring(1,
									childGroup[i].length());

							System.out.println("----" + roleElement);

							service.getPolicyDAO().addRole(roleElement, domainName
									+ "\\" + roleParentElement);
							if (roleSet.isEmpty()) {
								roleSet
										.add(domainName + "\\"
												+ roleElement);
								service.getPolicyDAO().addRoleSemantic(domainName
										+ "\\" + roleElement, "Group");
							}
							if (!roleSet.contains(domainName + "\\"
									+ roleElement)) {
								roleSet
										.add(domainName + "\\"
												+ roleElement);
								service.getPolicyDAO().addRoleSemantic(domainName
										+ "\\" + roleElement, "Group");
							}
						}
					}
					String[] childUser = s[2].split(">");
					for (int j = 0; j < childUser.length; j++) {
						if (!(childUser[j].contains("?"))) {
							roleElement = childUser[j];
							roleElement = childUser[j].substring(1,
									childUser[j].length());

							System.out.println("----" + roleElement);

							service.getPolicyDAO().addRole(roleElement, domainName
									+ "\\" + roleParentElement);
							if (roleSet.isEmpty()) {
								roleSet
										.add(domainName + "\\"
												+ roleElement);
								service.getPolicyDAO().addRoleSemantic(domainName
										+ "\\" + roleElement, "User");
							}
							if (!roleSet.contains(domainName + "\\"
									+ roleElement)) {
								roleSet
										.add(domainName + "\\"
												+ roleElement);
								service.getPolicyDAO().addRoleSemantic(domainName
										+ "\\" + roleElement, "User");
							}
						}
					}
				}

				if (str.equals("Inheritance:")) {
					authPFlag = 0;
					inhePFlag = 1;
					analysisInfo = "Parsing the inheritance policy...";
					service.getFrame().getMessageArea().append(analysisInfo + "\n");
					// System.out.println("授权策略");
				}
				if (authPFlag == 1) {
					// 将授权策略插入到对应的表中
					// tech.adtest.net\ly:KIRA\just for test3:RX:deny
					// s[0]是用户所属的域 S[1]用户名 s[2]主机名 s[3]文件夹名 s[4]活动名 s[5]措施
					String[] s = str.split(":|\\\\");
					// 列出每条策略的每个元组内容
					/*
					 * for(int i = 0; i< s.length; i++){
					 * System.out.println(s[i]); }
					 */
					// 策略表(policy)的插入
					CndPolicy cndPolicy = new CndPolicy();
					try {
						cndPolicy.setType(0);
						cndPolicy.setContext(s[2]);
						cndPolicy.setRole(s[0] + "\\" + s[1]);
						// System.out.println("S[0]=" + s[0] + "\n");
						cndPolicy.setView(s[3]);
						cndPolicy.setActivity(s[4]);
						cndPolicy.setMeasure(s[5]);
						cndPolicy.setOrganization(domainName);
						// System.out.println(domainName);

					} catch (NullPointerException ex) {
						ex.printStackTrace();
					}
					service.getPolicyDAO().addPolicy(cndPolicy);
					// context表的插入
					if (contextSet.isEmpty()) {
						contextSet.add(domainName + "\\" + s[2]);
						service.getPolicyDAO().addContext(s[2], domainName);
						// System.out.println("*********view表***********"+s[2]);
					}
					if (!contextSet.contains(domainName + "\\" + s[2])) {
						service.getPolicyDAO().addContext(s[2], domainName);
						contextSet.add(domainName + "\\" + s[2]);
						// System.out.println("**********view表**********"+s[2]);
					}
					// view表的插入
					if (viewSet.isEmpty()) {
						viewSet.add(domainName + "\\" + s[2] + "\\" + s[3]);
						service.getPolicyDAO().addView(s[3], domainName + "\\"
								+ s[2]);
						// System.out.println("*********view表***********"+s[2]);
					}
					if (!viewSet.contains(s[2] + "\\" + s[3])) {
						service.getPolicyDAO().addView(s[3], domainName + "\\"
								+ s[2]);
						viewSet.add(domainName + "\\" + s[2] + "\\" + s[3]);
						// System.out.println("**********view表**********"+s[2]);
					}
					// 语义表的插入
					// semaitic_role表 继承策略中体现

					// semantic_view表 文件夹是以主机+文件夹名来标识的
					if (semantic_viewSet.isEmpty()) {
						semantic_viewSet.add(domainName + "\\" + s[2]
								+ "\\" + s[3]);
						service.getPolicyDAO().addViewSemantic(domainName + "\\"
								+ s[2] + "\\" + s[3], "File");
						// System.out.println("*********semantic_view表***********"+s[2]);
					}
					if (!semantic_viewSet.contains(domainName + "\\" + s[2]
							+ "\\" + s[3])) {
						service.getPolicyDAO().addViewSemantic(domainName + "\\"
								+ s[2] + "\\" + s[3], "File");
						semantic_viewSet.add(domainName + "\\" + s[2]
								+ "\\" + s[3]);
						// System.out.println("**********semantic_view表**********"+s[2]);
					}
					// sematic_context表
					if (semantic_contextSet.isEmpty()) {
						semantic_contextSet.add(domainName + "\\" + s[2]);
						service.getPolicyDAO().addContextSemantic(domainName + "\\"
								+ s[2], "host");
						// System.out.println("**********sematic_context表**********"+s[1]);
					}
					if (!semantic_contextSet.contains(domainName + "\\"
							+ s[2])) {
						service.getPolicyDAO().addContextSemantic(domainName + "\\"
								+ s[2], "host");
						semantic_contextSet.add(domainName + "\\" + s[2]);
						// System.out.println("**********sematic_context表**********"+s[1]);
					}
				}
				if (str.equals("Authorization:")) {
					authPFlag = 1;
					inhePFlag = 0;
					analysisInfo = "Parsing the authorization policy...";
					service.getFrame().getMessageArea().append(analysisInfo + "\n");
					// System.out.println("授权策略");
				}
			}
		} catch (FileNotFoundException e) {
			System.out.println("找不到指定文件");
			JOptionPane.showMessageDialog(null, "File not Found Error",
					"Error", JOptionPane.ERROR_MESSAGE);
		} catch (IOException e) {
			System.out.println("读取文件失败");
			JOptionPane.showMessageDialog(null, "File read Error",
					"Error", JOptionPane.ERROR_MESSAGE);
		} finally {
			try {
				br.close();
				isr.close();
				fis.close();
				// 关闭的时候最好按照先后顺序关闭最后开的先关闭所以先关s,再关n,最后关m
			} catch (IOException e) {
				e.printStackTrace();
			}
		}

		analysisInfo = "policy analysis success!";
		service.getFrame().getMessageArea().append(analysisInfo + "\n");

	}

}


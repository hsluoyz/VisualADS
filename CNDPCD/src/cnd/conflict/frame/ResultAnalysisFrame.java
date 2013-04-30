package cnd.conflict.frame;

import java.awt.BorderLayout;
import java.awt.Container;
import java.awt.event.ActionEvent;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JScrollPane;
import javax.swing.JToolBar;

import cnd.conflict.dao.PolicyDAO;


public class ResultAnalysisFrame extends JFrame{

	/**
	 * @param args
	 */
	private PolicyDAO policyDAO = new PolicyDAO();
	JLabel label = new JLabel();
	Frame frame;

	public ResultAnalysisFrame(Frame frame) { // 构造函数
		super("结果分析图"); // 调用父类构造函数
		this.frame = frame;
		if (frame == null || frame.getLanguage() == 1) {
			setTitle("结果分析图");
			init("结果分析图");
		}
		else {
			setTitle("Result Analysis Diagram");
			init("Result Analysis Diagram");
		}
	}
	JScrollPane jScrollPane = new JScrollPane();
	public void init(String s) {
		Action[] actions = // Action数组,各种操作命令
		{ new ContextAction(s)};
		Container container = getContentPane(); // 得到容器
		container.add(createJToolBar(actions), BorderLayout.NORTH); // 增加工具栏
		
		container.add(label, BorderLayout.CENTER); // 增加文本窗格
		setSize(780, 700); // 设置窗口尺寸
		setVisible(true); // 设置窗口可视
		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE); // 关闭窗口时退出程序
	}

	private JToolBar createJToolBar(Action[] actions) { // 创建工具条
		JToolBar toolBar = new JToolBar(); // 实例化工具条
		for (int i = 0; i < actions.length; i++) {
			JButton bt = new JButton(actions[i]); // 实例化新的按钮
			bt.setRequestFocusEnabled(false); // 设置不需要焦点
			toolBar.add(bt); // 增加按钮到工具栏
		}
		return toolBar; // 返回工具栏
	}

	class ContextAction extends AbstractAction {
		public ContextAction(String s) {
			super(s);
		}

		public void actionPerformed(ActionEvent e) {
			 ImageIcon icon = new ImageIcon("img/result.jpg");
			 label.setIcon(icon);
			
		}
	}

	public static void main(String[] args) {
		// TODO Auto-generated method stub

		new ResultAnalysisFrame(null);
	}

}

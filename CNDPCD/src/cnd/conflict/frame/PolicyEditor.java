package cnd.conflict.frame;

import java.awt.BorderLayout;
import java.awt.CardLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.util.ArrayList;
import java.util.List;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.Box;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.JToolBar;

import org.jgraph.JGraph;

import cnd.conflict.dao.PolicyDAO;
import cnd.conflict.detect.RelationGraph;
import cnd.conflict.entity.CndPolicy;
import cnd.conflict.graph.GenerateGraph;

public class PolicyEditor extends JFrame implements ActionListener
{
	private static PolicyEditor policyEditor = null;
	private PolicyDAO policyDAO = new PolicyDAO();
	CardLayout myCard;
	JPanel pCenter;
	JToolBar toolBar;
	JButton buttonInsert,buttonUpdate,buttonExit;

	public PolicyEditor() {
		super("Policy Editor");		
	}

	private void addButtons(JToolBar toolBar) {
		 
		buttonInsert = new JButton("策略插入");
		buttonInsert.addActionListener(this);
		toolBar.add(buttonInsert); 
		buttonUpdate = new JButton("策略更新");
		buttonUpdate.addActionListener(this);
		toolBar.add(buttonUpdate);
		buttonExit = new JButton("退出");
		buttonExit.addActionListener(this);
		toolBar.add(buttonExit);
	}

	public void init() {
		myCard = new CardLayout();
		pCenter = new JPanel();
		pCenter.setLayout(myCard);
	
		toolBar = new JToolBar();         
		addButtons(toolBar); 
		Container container = getContentPane();
		container.add(toolBar, BorderLayout.NORTH);
		
		pCenter.add("策略插入", new InsertPanel());
		pCenter.add("策略更新", new UpdatePanel());
		
		this.add(pCenter, BorderLayout.CENTER);
		setSize(900, 400); // 设置窗口尺寸
		setVisible(true); // 设置窗口可视
		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE); // 关闭窗口时退出程序
	}
	
	public static PolicyEditor getInstance() {
		if (policyEditor == null) {
			policyEditor = new PolicyEditor();
		}
		return policyEditor;
	}

	public void actionPerformed(ActionEvent e) {
        if(e.getSource() == buttonInsert){
            myCard.show(pCenter, "策略插入");
        }
        if(e.getSource() == buttonUpdate){
        	myCard.show(pCenter, "策略更新");
        }
		if(e.getSource() == buttonExit){
			PolicyEditor.this.dispose();
		}
	}

}

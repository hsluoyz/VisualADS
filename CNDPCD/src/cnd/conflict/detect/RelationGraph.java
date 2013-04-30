package cnd.conflict.detect;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Insets;
import java.awt.event.ActionEvent;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.awt.geom.Rectangle2D;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Hashtable;
import java.util.List;
import java.util.Map;
import java.util.Set;

import javax.swing.AbstractAction;
import javax.swing.Action;
import javax.swing.BorderFactory;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JToggleButton;
import javax.swing.JToolBar;
import javax.swing.border.Border;

import org.jgraph.JGraph;
import org.jgraph.graph.ConnectionSet;
import org.jgraph.graph.DefaultEdge;
import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.DefaultGraphModel;
import org.jgraph.graph.DefaultPort;
import org.jgraph.graph.GraphConstants;
import org.jgraph.graph.GraphModel;

import cnd.conflict.dao.PolicyDAO;
import cnd.conflict.frame.Frame;
import cnd.conflict.graph.Corner;
import cnd.conflict.graph.GenerateGraph;
import cnd.conflict.graph.Rule;
import cnd.conflict.test.ScrollablePicture;


/**
 * @author YC
 * 
 */
public class RelationGraph extends JFrame {
	private Frame frame;
	private Rule columnView;
	private Rule rowView;
	
	private static RelationGraph relationGraph = null;

	private PolicyDAO policyDAO = new PolicyDAO();
	JLabel label = new JLabel();

	private RelationGraph(String s) { // 构造函数
		super(s); // 调用父类构造函数
	}

	JScrollPane jScrollPane = new JScrollPane();

	public void init(Frame frame) {
		this.frame = frame;
		
		Container container = getContentPane(); // 得到容器
		container.removeAll();
		if (frame == null || frame.getLanguage() == 1) {
			relationGraph.setTitle("元组关系图\n");
			Action[] actions = // Action数组,各种操作命令
				{ new ContextAction("上下文关系图"), new RoleAction("角色关系图"), new ViewAction("视图关系图"),
				  new ActivityAction("活动关系图"), new MeasureAction("手段关系图"), new ExitAction("退出") };
			container.add(createJToolBar(actions), BorderLayout.NORTH); // 增加工具栏
		}
		else {
			relationGraph.setTitle("Tuple Graph\n");
			Action[] actions = // Action数组,各种操作命令
				{ new ContextAction("Context Graph"), new RoleAction("Role Graph"),
				  new ViewAction("View Graph"), new ActivityAction("Activity Graph"),
				  new MeasureAction("Measure Graph"), new ExitAction("Close") };
			container.add(createJToolBar(actions), BorderLayout.NORTH); // 增加工具栏
		}
		
		

		JPanel pLeft = new JPanel(), pRight = new JPanel(),
		// pTop = new JPanel(),
		pBottom = new JPanel();

		Border loweredbevel = BorderFactory.createLoweredBevelBorder();
		RelationGraph.this.jScrollPane.setViewportBorder(loweredbevel); 
		//设置图像的标尺
        // Create the row and column headers.
        columnView = new Rule(Rule.HORIZONTAL, true);
        columnView.setPreferredWidth(400);
        rowView = new Rule(Rule.VERTICAL, true);
        rowView.setPreferredHeight(400);
        
        JToggleButton isMetric = new JToggleButton("cm", true);
        // Create the corners.
        JPanel butCorner = new JPanel();
        isMetric = new JToggleButton("cm", true);
        isMetric.setFont(new Font("SansSerif", Font.PLAIN, 11));
        isMetric.setMargin(new Insets(2,2,2,2));
        isMetric.addItemListener(new UnitsListener());
        butCorner.add(isMetric); //Use the default FlowLayout
                
        jScrollPane.setPreferredSize(new Dimension(300, 250));
    
        jScrollPane.setColumnHeaderView(columnView);	        
        jScrollPane.setRowHeaderView(rowView);
        jScrollPane.setCorner(JScrollPane.UPPER_LEFT_CORNER, 
                                    butCorner);
        jScrollPane.setCorner(JScrollPane.LOWER_LEFT_CORNER,
                                    new Corner());
        jScrollPane.setCorner(JScrollPane.UPPER_RIGHT_CORNER,
                                    new Corner());
				
		container.add(jScrollPane, BorderLayout.CENTER); // 增加文本窗格
	 
		container.add(pLeft, BorderLayout.WEST);
		container.add(pRight, BorderLayout.EAST);
		// container.add(pTop, BorderLayout.NORTH);
		container.add(pBottom, BorderLayout.SOUTH);

		setSize(500, 500); // 设置窗口尺寸
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
			
			JGraph graph = new GenerateGraph(frame).paintGraph("context");
	        RelationGraph.this.jScrollPane.setViewportView(graph);
						
		}
	}
	
    class UnitsListener implements ItemListener {
        public void itemStateChanged(ItemEvent e) {
            if (e.getStateChange() == ItemEvent.SELECTED) {
                // Turn it to metric.
                rowView.setIsMetric(true);
                columnView.setIsMetric(true);
            } else {
                // Turn it to inches.
                rowView.setIsMetric(false);
                columnView.setIsMetric(false);
            }
            //picture.setMaxUnitIncrement(rowView.getIncrement());
        }
    }

	class RoleAction extends AbstractAction {
		public RoleAction(String s) {
			super(s);
		}

		public void actionPerformed(ActionEvent e) {
			/*
			 * ImageIcon icon = new ImageIcon("img/role.gif");
			 * label.setIcon(icon);
			 */

			JGraph graph = new GenerateGraph(frame).paintGraph("role");
			RelationGraph.this.jScrollPane.setViewportView(graph);
		}
	}

	class ViewAction extends AbstractAction {
		public ViewAction(String s) {
			super(s);
		}

		public void actionPerformed(ActionEvent e) {
			/*
			 * ImageIcon icon = new ImageIcon("img/view.gif");
			 * label.setIcon(icon);
			 */
			JGraph graph = new GenerateGraph(frame).paintGraph("view");
			RelationGraph.this.jScrollPane.setViewportView(graph);
		}
	}

	class ExitAction extends AbstractAction {
		public ExitAction(String s) {
			super(s);
		}

		public void actionPerformed(ActionEvent e) {
			// System.exit(0);
			RelationGraph.this.dispose();
		}
	}

	class ActivityAction extends AbstractAction {
		public ActivityAction(String s) {
			super(s);
		}

		public void actionPerformed(ActionEvent e) {
			/*
			 * ImageIcon icon = new ImageIcon("img/activity.gif");
			 * label.setIcon(icon);
			 */
			JGraph graph = new GenerateGraph(frame).paintGraph("activity");
			RelationGraph.this.jScrollPane.setViewportView(graph);
		}
	}

	class MeasureAction extends AbstractAction {
		public MeasureAction(String s) {
			super(s);
		}

		public void actionPerformed(ActionEvent e) {
			/*
			 * ImageIcon icon = new ImageIcon("img/measure.gif");
			 * label.setIcon(icon);
			 */
		}
	}

	/**
	 * 获取单态实例
	 * 
	 * @return RelationGraph
	 */
	public static RelationGraph getInstance(Frame frame) {
		if (relationGraph == null) {
			relationGraph = new RelationGraph("元组关系图");
		}
		if (frame == null || frame.getLanguage() == 1)
			relationGraph.setTitle("元组关系图\n");
		else
			relationGraph.setTitle("Tuple Graph\n");
		return relationGraph;
	}

	public static void main(String[] args) {
		new RelationGraph("元组关系图");
	}
}

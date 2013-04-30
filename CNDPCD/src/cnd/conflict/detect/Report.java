package cnd.conflict.detect;

import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Container;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;

import javax.swing.JFileChooser;
import javax.swing.JFrame;
import javax.swing.JMenu;
import javax.swing.JMenuBar;
import javax.swing.JMenuItem;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.text.BadLocationException;

import cnd.conflict.frame.Frame;


public class Report extends JFrame {

	/**
	 * 
	 */
	private static Report notepadSample = null;
	BorderLayout borderLayout = new BorderLayout();
	JTextArea jTextArea = new JTextArea();
	JScrollPane jScrollPane = new JScrollPane(jTextArea);
	JMenuBar jMenuBar = new JMenuBar();
	JMenu jMenu1 = new JMenu();
	JMenu jMenu2 = new JMenu();
	JMenuItem jMenuItem1 = new JMenuItem();
	JMenuItem jMenuItem2 = new JMenuItem();
	JMenuItem jMenuItem3 = new JMenuItem();
	JFileChooser jFileChooser = new JFileChooser();
	
	
	public void init(Frame frame)throws Exception{
		if (frame == null || frame.getLanguage() == 1) {
			this.setTitle("冲突检测报告");
			this.setSize(600, 400);
			this.setVisible(true);
			this.getContentPane().setLayout(borderLayout);
			jTextArea.setText(frame.getMessageArea().getText());
			this.setJMenuBar(jMenuBar);
			jMenu1.setText("文件");
			jMenu2.setText("关于");
			
			jMenuItem2.setText("保存文件");
		}
		else {
			this.setTitle("Conflict Detection Report");
			this.setSize(600, 400);
			this.setVisible(true);
			this.getContentPane().setLayout(borderLayout);
			jTextArea.setText(frame.getMessageArea().getText());
			this.setJMenuBar(jMenuBar);
			jMenu1.setText("File");
			jMenu2.setText("About");
			
			jMenuItem2.setText("Save");
		}

		jMenuItem2.addActionListener(new ActionListener(){

			public JTextField getTextField(Container c){
				JTextField textField = null; 
				for (int i = 0; i < c.getComponentCount(); i++) {
					 Component cnt = c.getComponent(i); 
					 if (cnt instanceof JTextField) { 
						 return (JTextField) cnt; 
					 }
					 if (cnt instanceof Container) {
						 textField = getTextField((Container) cnt);                
						 if (textField != null) {                    
							 return textField;  
							 } 
					 }
				}
				return textField; 
			}
		
			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				jFileChooser.setPreferredSize(new Dimension(400,300));
				JTextField text;
				
				text=getTextField(jFileChooser);
				text.setText("report.txt");

				int returnVal = jFileChooser.showSaveDialog(Report.this); //显示保存文件对话框
				if (returnVal == JFileChooser.APPROVE_OPTION) {  //点击对话框中保存按钮
				    File f = jFileChooser.getSelectedFile(); //得到选择的文件
				  
				    try {
						FileWriter outOne = new FileWriter(f);
						BufferedWriter outTwo = new BufferedWriter(outOne);
						
						jTextArea.append("\0"); 
		                for (int i = 0; i < jTextArea.getLineCount(); i++) { 
		                    try { 
		                        outTwo.write(jTextArea.getText(jTextArea    
		                                .getLineStartOffset(i), jTextArea 
		                                .getLineEndOffset(i) 
		                                - jTextArea.getLineStartOffset(i) - 1)); 
		                        outTwo.newLine();
		                    } catch (BadLocationException ex) { 
		                        ex.printStackTrace(); 
		                    }
		                }
		                
		                outTwo.close();
		                outOne.close();

						
					} catch (IOException e1) {
						// TODO Auto-generated catch block
						e1.printStackTrace();
					}
				   }
			}
			
		});
		jMenuItem3.setText("退出");
		jMenuItem3.addActionListener(new ActionListener(){

			@Override
			public void actionPerformed(ActionEvent e) {
				// TODO Auto-generated method stub
				jMenuItem3_actionPerformed(e);
			}
			
		});
		jFileChooser.setPreferredSize(new Dimension(0,0));
		this.getContentPane().add(jScrollPane, BorderLayout.CENTER);
		this.getContentPane().add(jFileChooser, BorderLayout.SOUTH);
		jMenuBar.add(jMenu1);
		jMenuBar.add(jMenu2);
		jMenu1.add(jMenuItem2);
		jMenu1.add(jMenuItem3);
		this.addWindowListener(new WindowAdapter(){
			public void windowClosing(WindowEvent e){
//				System.exit(0);
				Report.this.dispose();
			}
		});
	}
	
	protected void jMenuItem3_actionPerformed(ActionEvent e) {
		// TODO Auto-generated method stub
//		System.exit(0);
		this.dispose();
		
	}
	protected void jMenuItem2_actionPerformed(ActionEvent e) {
		// TODO Auto-generated method stub
		
	}
	
	
	 public static Report getInstance() {
			if (notepadSample == null) {
				notepadSample = new Report();
			}
			return notepadSample;
		}
	
	public static void main(String[] args){
		Report notepadSample = new Report();
		
	}
}

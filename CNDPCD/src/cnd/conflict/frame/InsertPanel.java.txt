package cnd.conflict.frame;

import java.awt.Component;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.ItemEvent;
import java.awt.event.ItemListener;
import java.util.ArrayList;
import java.util.List;

import javax.swing.Box;
import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextArea;

import cnd.conflict.dao.PolicyDAO;
import cnd.conflict.entity.CndPolicy;

public class InsertPanel extends JPanel implements ActionListener, ItemListener{
	    private String policyFieldArr[] = {"PolicyID","Type","Context","Role","View","Activity","Measure"};
	    private String comboType[] = {"protect","detect","response"};
	   // private String comboItem[] = {"A","B","C","D","E","F"};
	    private List<String> contextList = null;
	    private List<String> roleList = null;
	    private List<String> viewList = null;
	    private List<String> activityList = null;
	    private List<String> measureList = null;
	    
	    private PolicyDAO policyDAO = new PolicyDAO();
	    
	    public List<JComboBox> comBox = new ArrayList<JComboBox>();
	    
	    private Box baseBoxH_1, baseBoxH_2, baseBoxV;
	    private JTextArea policyArea;
	    private JButton insertButton, cancelButton;
	    
	    public InsertPanel(){
	    	init();
	    }
	    
	    public void init(){
	    	//ruleEditorPanel = new JPanel();
	    	baseBoxV = Box.createVerticalBox();
	    	baseBoxH_1 = Box.createHorizontalBox();
	    	baseBoxH_2 = Box.createHorizontalBox();
	    	
	    	//System.out.println(policyFieldArr.length-1);
	    	
	    	
	    	policyArea = new JTextArea(10,2);
	    	insertButton = new JButton("insert");
	    	cancelButton = new JButton("cancel");
	    	/*cancelButton.addActionListener(new ActionListener(){

				@Override
				public void actionPerformed(ActionEvent e) {
					// TODO Auto-generated method stub
					PolicyEditor.this.dispose();
				}
				
			});*/
	    	
	    	for(int i = 0; i< policyFieldArr.length ; i++){
	    		Box boxV = Box.createVerticalBox();
	    		boxV.add(new JLabel(policyFieldArr[i]));
	    		if( i == 0){
	    			boxV.add(new JLabel("Auto_increment"));
	    			baseBoxH_1.add(boxV);
	    			baseBoxH_1.add(Box.createHorizontalStrut(10));
	    		}                                             
	    		else{
	    			 comBox.add(new JComboBox());
	    			 if(i == 1){                                       //0--Type;   
	    				 for(int k =0; k< comboType.length; k++){
	    	    				comBox.get(i-1).addItem(comboType[k]);       
	    	    			}
	    			 }
	    			 if(i == 2){
	    				 try{
	    					 contextList = policyDAO.getSemanticContext();
	        				 for(int k = 0;k < contextList.size();k++){
	            				comBox.get(i-1).addItem(contextList.get(k));  
	            			   }  
	    				 }catch(NullPointerException e){
	    					 e.printStackTrace();
	    				 }
	    				 
	    			}
	    				
	    			 if(i == 3){
	    				 roleList = policyDAO.getSemanticRole();
	    				 for(int k = 0;k < roleList.size();k++){
	    					 comBox.get(i-1).addItem(roleList.get(k));  
	    				 }
	    			 }
	    			 if(i == 4){
	    				 viewList = policyDAO.getSemanticView();
	    				 for(int k = 0;k < viewList.size();k++){
	    					 comBox.get(i-1).addItem(viewList.get(k));  
	    				 }
	    			 }
	    			 if(i == 5){
	    				 activityList = policyDAO.getSemanticActivity();
	    				 for(int k = 0;k < activityList.size();k++){
	    					 comBox.get(i-1).addItem(activityList.get(k));  
	    				 }
	    			 }
	    			 if(i == 6){
	    				 measureList = policyDAO.getSemanticMeasureByType(0);
	    				 for(int k = 0;k < measureList.size();k++){
	    					 comBox.get(i-1).addItem(measureList.get(k));  
	    				 }
	    			 }
	    			 
	    			
	    			 boxV.add(comBox.get(i-1));
	    			 baseBoxH_1.add(boxV);
	    			 baseBoxH_1.add(Box.createHorizontalStrut(10));
	    		}	
	    	}
	    	
	    	baseBoxH_2.add(Box.createHorizontalStrut(400));
	    	baseBoxH_2.add(insertButton);
	    	baseBoxH_2.add(Box.createHorizontalStrut(10));
	    	baseBoxH_2.add(cancelButton);
	    	
	    	baseBoxV.add(baseBoxH_1);
	    	baseBoxV.add(Box.createVerticalStrut(20));
	    	baseBoxV.add(policyArea);
	    	baseBoxV.add(Box.createVerticalStrut(20));
	    	baseBoxV.add(baseBoxH_2);
	      //  this.add(ruleEditorPanel);
	        this.add(baseBoxV);
	        
	        insertButton.addActionListener(this);
	        cancelButton.addActionListener(this);
	       
	        comBox.get(0).addItemListener(this);
	    	//setSize(700,260); // 设置窗口尺寸
			//this.setVisible(true);// 设置窗口可视\
			this.validate();
			//setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE); // 关闭窗口时退出程序
	    }

		public void actionPerformed(ActionEvent e) {
			if(e.getSource() == insertButton){
				CndPolicy cndPolicy = new CndPolicy();
			
				String s = comBox.get(0).getSelectedItem().toString();
				if(s.equals("protect")){
					cndPolicy.setType(0);
				}
				if(s.equals("detect")){
					cndPolicy.setType(1);
				}
				if(s.equals("response")){
					cndPolicy.setType(2);
				}
				try{
					cndPolicy.setContext(comBox.get(1).getSelectedItem().toString());
					cndPolicy.setRole(comBox.get(2).getSelectedItem().toString());
					cndPolicy.setView(comBox.get(3).getSelectedItem().toString());
					cndPolicy.setActivity(comBox.get(4).getSelectedItem().toString());
					cndPolicy.setMeasure(comBox.get(5).getSelectedItem().toString());
				}catch(NullPointerException ex){
					ex.printStackTrace();
				}
				
				int flag = new PolicyDAO().addPolicy(cndPolicy);
				if(flag == 1){
					StringBuffer strBudffer = new StringBuffer();
					strBudffer.append(s+":");
					strBudffer.append(cndPolicy.getContext()+" ");
					strBudffer.append(cndPolicy.getRole()+" ");
					strBudffer.append(cndPolicy.getView()+" ");
					strBudffer.append(cndPolicy.getActivity()+" ");
					strBudffer.append(cndPolicy.getMeasure()+" ");
					strBudffer.append("\n");
					strBudffer.append("insert succeed !!");
					policyArea.setText(strBudffer.toString());
				}
				
			}
			
		}

		public void itemStateChanged(ItemEvent e) {
			if(e.getSource() ==  comBox.get(0)){
				String str = (String) comBox.get(0).getSelectedItem();
				if(str.equals("protect")){
					comBox.get(5).removeAllItems();
					 measureList = policyDAO.getSemanticMeasureByType(0);
					 for(int k = 0;k < measureList.size();k++){
						 comBox.get(5).addItem(measureList.get(k));  
					 }
				}
				if(str.equals("detect")){
					comBox.get(5).removeAllItems();
					 measureList = policyDAO.getSemanticMeasureByType(1);
					 for(int k = 0;k < measureList.size();k++){
						 comBox.get(5).addItem(measureList.get(k));  
					 }
				}
				if(str.equals("response")){
					 comBox.get(5).removeAllItems();
					 measureList = policyDAO.getSemanticMeasureByType(2);
					 for(int k = 0;k < measureList.size();k++){
						 comBox.get(5).addItem(measureList.get(k));  
					 }
				}
				
				
			}
			
		}
}

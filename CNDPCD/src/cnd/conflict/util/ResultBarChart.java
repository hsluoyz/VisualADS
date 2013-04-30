package cnd.conflict.util;

import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.util.List;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartUtilities;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.data.category.CategoryDataset;
import org.jfree.data.category.DefaultCategoryDataset;

import cnd.conflict.dao.PolicyDAO;
import cnd.conflict.entity.Result;

public class ResultBarChart {

	/**
	 * @param args
	 */
	
	private PolicyDAO policyDAO = new PolicyDAO();
	
	public ResultBarChart(){
		init();
	}
	public void init(){
		CategoryDataset  dataset = getDataSet2();  
        JFreeChart chart = ChartFactory.createBarChart(  
                "Result analysis grpah", // 标题  
                "Amount of policy",       // 目录轴（水平）  
                "Amount of conflict",       // 数值轴（垂直）  
                dataset,    // 数据集  
                PlotOrientation.VERTICAL,   // 图表方向（水平/垂直）  
                true,       // 是否显示图例（对于简单的柱状图是必须的）  
                false,      // 是否生成工具  
                false       // 是否生成 url 链接  
        );  
  
        FileOutputStream fos_jpg = null;  
        try {  
            try {
				fos_jpg = new FileOutputStream("./img/result.jpg");
			} catch (FileNotFoundException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}     // 图片的输出目录  
            try {
				ChartUtilities.writeChartAsJPEG(fos_jpg, (float) 0.9, chart, 700, 550, null);
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}  
        } finally {  
            try {  
                fos_jpg.close();  
            } catch (Exception e) {  
                e.printStackTrace();  
            }  
        }  
	}
	 public  CategoryDataset  getDataSet2() {  
	        DefaultCategoryDataset dataset = new DefaultCategoryDataset();

	        List<Result> resultList = policyDAO.getResultList();
	        for(Result element:resultList){
	        	if(element.getPolicyAmount() == 5){
	        		dataset.addValue(element.getUnrelation(), "UNRELATION", String.valueOf(element.getPolicyAmount()));
		        	dataset.addValue(element.getSyntaxconflict(), "SYNTAXCONFLICT", String.valueOf(element.getPolicyAmount()));
		        	dataset.addValue(element.getInclusivematch(), "INCLUSIVEMATCH", String.valueOf(element.getPolicyAmount()));
		        	dataset.addValue(element.getPartialmatch(), "PARTIALMATCH", String.valueOf(element.getPolicyAmount()));
	        	}
	        	if(element.getPolicyAmount() == 12){
	        		dataset.addValue(element.getUnrelation(), "UNRELATION", String.valueOf(element.getPolicyAmount()));
		        	dataset.addValue(element.getSyntaxconflict(), "SYNTAXCONFLICT", String.valueOf(element.getPolicyAmount()));
		        	dataset.addValue(element.getInclusivematch(), "INCLUSIVEMATCH", String.valueOf(element.getPolicyAmount()));
		        	dataset.addValue(element.getPartialmatch(), "PARTIALMATCH", String.valueOf(element.getPolicyAmount()));
	        	}
	        	if(element.getPolicyAmount() == 8){
	        		dataset.addValue(element.getUnrelation(), "UNRELATION", String.valueOf(element.getPolicyAmount()));
		        	dataset.addValue(element.getSyntaxconflict(), "SYNTAXCONFLICT", String.valueOf(element.getPolicyAmount()));
		        	dataset.addValue(element.getInclusivematch(), "INCLUSIVEMATCH", String.valueOf(element.getPolicyAmount()));
		        	dataset.addValue(element.getPartialmatch(), "PARTIALMATCH", String.valueOf(element.getPolicyAmount()));
	        	}
	    
	        	
	        }
	        
	        for(Result element:resultList){
	        	
	        	if(element.getPolicyAmount() == 16){
	        		dataset.addValue(element.getUnrelation(), "UNRELATION", String.valueOf(element.getPolicyAmount()));
		        	dataset.addValue(element.getSyntaxconflict(), "SYNTAXCONFLICT", String.valueOf(element.getPolicyAmount()));
		        	dataset.addValue(element.getInclusivematch(), "INCLUSIVEMATCH", String.valueOf(element.getPolicyAmount()));
		        	dataset.addValue(element.getPartialmatch(), "PARTIALMATCH", String.valueOf(element.getPolicyAmount()));
	        	}
	        	
	        	
	        }
	        
	        for(Result element:resultList){
	        	
	        	if(element.getPolicyAmount() == 20){
	        		dataset.addValue(element.getUnrelation(), "UNRELATION", String.valueOf(element.getPolicyAmount()));
		        	dataset.addValue(element.getSyntaxconflict(), "SYNTAXCONFLICT", String.valueOf(element.getPolicyAmount()));
		        	dataset.addValue(element.getInclusivematch(), "INCLUSIVEMATCH", String.valueOf(element.getPolicyAmount()));
		        	dataset.addValue(element.getPartialmatch(), "PARTIALMATCH", String.valueOf(element.getPolicyAmount()));
	        	}
	        	
	        	
	        }
	        
	        
	      /*  dataset.addValue(300, "UNRELATION", "10"); 
	        dataset.addValue(600, "SYNTAXCONFLICT", "10"); 
	        dataset.addValue(500, "INCLUSIVEMATCH", "10"); 
	        dataset.addValue(400, "PARTIALMATCH", "10"); 
	        
	        dataset.addValue(300, "UNRELATION", "20"); 
	        dataset.addValue(600, "SYNTAXCONFLICT", "20"); 
	        dataset.addValue(500, "INCLUSIVEMATCH", "20"); 
	        dataset.addValue(400, "PARTIALMATCH", "20"); 
	        
	        dataset.addValue(300, "UNRELATION", "30"); 
	        dataset.addValue(600, "SYNTAXCONFLICT", "30"); 
	        dataset.addValue(500, "INCLUSIVEMATCH", "30"); 
	        dataset.addValue(400, "PARTIALMATCH", "30"); 
	        
	        dataset.addValue(300, "UNRELATION", "40"); 
	        dataset.addValue(600, "SYNTAXCONFLICT", "40"); 
	        dataset.addValue(500, "INCLUSIVEMATCH", "40"); 
	        dataset.addValue(400, "PARTIALMATCH", "40"); 
	        
	        dataset.addValue(300, "UNRELATION", "50"); 
	        dataset.addValue(600, "SYNTAXCONFLICT", "50"); 
	        dataset.addValue(500, "INCLUSIVEMATCH", "50"); 
	        dataset.addValue(400, "PARTIALMATCH", "50"); */
	        
	        
	       
	        return dataset;  
	    }  
	
	public static void main(String[] args) {
		// TODO Auto-generated method stub

		new ResultBarChart();
	}
	  
	   
	}



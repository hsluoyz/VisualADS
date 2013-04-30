package cnd.conflict.graph;

import java.awt.Color;
import java.awt.geom.Rectangle2D;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Hashtable;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;

import org.eclipse.draw2d.geometry.Insets;
import org.eclipse.draw2d.graph.DirectedGraph;
import org.eclipse.draw2d.graph.DirectedGraphLayout;
import org.eclipse.draw2d.graph.Edge;
import org.eclipse.draw2d.graph.Node;
import org.jgraph.JGraph;
import org.jgraph.graph.ConnectionSet;
import org.jgraph.graph.DefaultEdge;
import org.jgraph.graph.DefaultGraphCell;
import org.jgraph.graph.DefaultGraphModel;
import org.jgraph.graph.GraphConstants;
import org.jgraph.graph.GraphModel;

import cnd.conflict.bean.EdgeBean;
import cnd.conflict.bean.NodeBean;
import cnd.conflict.bean.UnionEdge;
import cnd.conflict.dao.PolicyDAO;
import cnd.conflict.frame.Frame;
import cnd.conflict.service.Service;
import cnd.conflict.util.DataBaseConn;

import com.realpersist.gef.layout.NodeJoiningDirectedGraphLayout;

/**
 * @author YC
 * 
 */
public class GenerateGraph {
	private Frame frame = null;

	public Map gefNodeMap = null;

	public Map graphNodeMap = null;

	public List edgeList = null;

	private PolicyDAO policyDAO = new PolicyDAO();
	// 引入SERVICE对象 得到文件解析后 关于域的信息
	private Service service; // = Service.getServiceInstance(frame);

	DirectedGraph directedGraph = null;

	JGraph graph = null;

	private DataBaseConn dataBaseConn = DataBaseConn.getDataBaseConnInstance();

	public GenerateGraph(Frame frame) {
		this.frame = frame;
		service = Service.getServiceInstance(frame);
	}

	public JGraph paintGraph(String elementName) {
		try {

			GraphModel model = new DefaultGraphModel();
			graph = new JGraph(model);
			graph.setSelectionEnabled(true);

			HashMap<String, String> hashMap = null;

			List<EdgeBean> edgeBeanList = new ArrayList<EdgeBean>();

			if ("policy" == dataBaseConn.getUrl_database()) {
				List<String> eList = null;
				if (elementName.equals("context")) {
					eList = policyDAO.getSemanticContext();
					hashMap = policyDAO.getContextMap();
				}
				if (elementName.equals("role")) {
					eList = policyDAO.getSemanticRole();
					hashMap = policyDAO.getRoleMap();
				}
				if (elementName.equals("view")) {
					eList = policyDAO.getSemanticView();
					hashMap = policyDAO.getViewMap();
				}
				if (elementName.equals("activity")) {
					eList = policyDAO.getSemanticActivity();
					hashMap = policyDAO.getActivityMap();
				}

				List<NodeBean> dList = new ArrayList<NodeBean>();

				for (String element : eList) {
					dList.add(new NodeBean(element));
				}
				List<Map.Entry<String, String>> mapList = null;
				mapList = new ArrayList<Map.Entry<String, String>>(hashMap
						.entrySet());
				for (Map.Entry<String, String> elementEntry : mapList) {
					String str = elementEntry.getKey().substring(
							elementEntry.getKey().indexOf(':') + 1);
					NodeBean start = dList.get(eList.indexOf(str));
					NodeBean end = dList.get(eList.indexOf(elementEntry
							.getValue()));
					EdgeBean edge = new EdgeBean(start, end, new Long(20));
					edgeBeanList.add(edge);
				}

			}
			// 在文件数据库中创建的点属于不同的类型 一共有4种
			if ("fileservices" == dataBaseConn.getUrl_database()) {
				// 关系表的一条内容中 pre对应ele post对应parentele 创建两个对应的节点链完全是为了解决唯一标示的问题
				// 节点中的字符串可以唯一标识
				List<String> eList_pre = null;
				List<String> eList_post = null;

				List<String> eList_host = null;
				// 节点中的字符串是最终显示效果
				List<NodeBean> dList_pre = new ArrayList<NodeBean>();
				List<NodeBean> dList_post = new ArrayList<NodeBean>();

				List<NodeBean> dList_host = new ArrayList<NodeBean>();

				if (elementName.equals("context")) {
					eList_pre = policyDAO.getSemanticContext();
					eList_post = generateDomainStrNode();
					// 修正节点显示的字符串
					for (String element : eList_pre) {
						int index = element.lastIndexOf("\\");
						element = element.substring(index + 1);
						dList_pre.add(new NodeBean(element));
					}
					for (String element : eList_post) {
						dList_post.add(new NodeBean(element));
					}
					// 节点生成完毕后 获得继承关系
					hashMap = policyDAO.getContextMap();

					List<Map.Entry<String, String>> mapList = null;
					mapList = new ArrayList<Map.Entry<String, String>>(hashMap
							.entrySet());

					for (Map.Entry<String, String> elementEntry : mapList) {
						// 修正从数据库中取出的字符串
						String str = (elementEntry.getKey()).replaceAll(":",
								"\\\\");
						NodeBean end = dList_pre.get(eList_pre.indexOf(str));
						NodeBean start = dList_post.get(eList_post
								.indexOf(elementEntry.getValue()));
						EdgeBean edge = new EdgeBean(start, end, new Long(20));
						edgeBeanList.add(edge);
					}
				}
				if (elementName.equals("role")) {
					eList_pre = policyDAO.getSemanticRole();
					eList_post = generateDomainStrNode();
					// 修正节点显示的字符串
					for (String element : eList_pre) {
						int index = element.lastIndexOf("\\");
						element = element.substring(index + 1);
						dList_pre.add(new NodeBean(element));
					}
					for (String element : eList_post) {
						dList_post.add(new NodeBean(element));
					}
					// 节点生成完毕后 获得继承关系
					hashMap = policyDAO.getRoleMap();

					List<Map.Entry<String, String>> mapList = null;
					mapList = new ArrayList<Map.Entry<String, String>>(hashMap
							.entrySet());

					for (Map.Entry<String, String> elementEntry : mapList) {
				    // 修正从数据库中取出的字符串
					int eleParentIndex = elementEntry.getValue().indexOf("\\");
                    String str_domain = elementEntry.getValue().substring(0, eleParentIndex);                  
                    String eleParent = elementEntry.getValue();
                    int elParentIndex = elementEntry.getKey().indexOf(":");
                    String eleName = str_domain + "\\" + elementEntry.getKey().substring(elParentIndex + 1);
                    System.out.println("域名"+str_domain);
                    System.out.println("组名"+eleParent);
                    System.out.println("用户名"+eleName);
                                     
                    NodeBean end = dList_pre.get(eList_pre.indexOf(eleName));
					NodeBean mid = dList_pre.get(eList_pre
							.indexOf(eleParent));

					NodeBean start = dList_post.get(eList_post
							.indexOf(str_domain));

					EdgeBean edged_h = new EdgeBean(start, mid,
							new Long(20));
					EdgeBean edgeh_f = new EdgeBean(mid, end, new Long(20));
					edgeBeanList.add(edged_h);
					edgeBeanList.add(edgeh_f);
					}
					//获得信任关系 得到域名之间的连接方式
					NodeBean positive = dList_post.get(eList_post
							.indexOf("tech.adtest.net"));
					NodeBean negative = dList_post.get(eList_post
							.indexOf("sales.adtest.net"));
					EdgeBean edged = new EdgeBean(positive, negative,
							new Long(20));
					edgeBeanList.add(edged);
				}
				if (elementName.equals("view")) {
					// 文件夹的唯一标识
					eList_pre = policyDAO.getSemanticView();
					// 主机的唯一标识
					eList_host = policyDAO.getSemanticContext();
					// 域的唯一标识
					eList_post = generateDomainStrNode();

					// 修正节点显示的字符串
					for (String element : eList_pre) {
						int index = element.lastIndexOf("\\");
						element = element.substring(index + 1);
						System.out.println("文件夹" + element);
						dList_pre.add(new NodeBean(element));
					}
					for (String element : eList_host) {
						int index = element.lastIndexOf("\\");
						element = element.substring(index + 1);
						System.out.println("主机" + element);
						dList_host.add(new NodeBean(element));
					}
					for (String element : eList_post) {
						System.out.println("域名" + element);
						dList_post.add(new NodeBean(element));
					}

					hashMap = policyDAO.getViewMap();

					List<Map.Entry<String, String>> mapList = null;
					mapList = new ArrayList<Map.Entry<String, String>>(hashMap
							.entrySet());

					for (Map.Entry<String, String> elementEntry : mapList) {
						// 修正从数据库中取出的字符串
						String str = (elementEntry.getKey()).replaceAll(":",
								"\\\\");
						String str_host = elementEntry.getValue();
						System.out.println("~~~~~~-----------~~~~~" + str);
						System.out.println("~~~~~~~~~~~~~~~~~~~~~~" + str_host);

						int hostIndex = elementEntry.getValue().indexOf("\\");
						String str_domain = elementEntry.getValue().substring(
								0, hostIndex);
						System.out.println("~~~~***********~~~~~~~"
								+ str_domain);

						NodeBean end = dList_pre.get(eList_pre.indexOf(str));
						NodeBean mid = dList_host.get(eList_host
								.indexOf(str_host));

						NodeBean start = dList_post.get(eList_post
								.indexOf(str_domain));

						EdgeBean edged_h = new EdgeBean(start, mid,
								new Long(20));
						EdgeBean edgeh_f = new EdgeBean(mid, end, new Long(20));
						edgeBeanList.add(edged_h);
						edgeBeanList.add(edgeh_f);
					}
					
					//获得信任关系 得到域名之间的连接方式
					NodeBean positive = dList_post.get(eList_post
							.indexOf("tech.adtest.net"));
					NodeBean negative = dList_post.get(eList_post
							.indexOf("sales.adtest.net"));
					EdgeBean edged = new EdgeBean(positive, negative,
							new Long(20));
					edgeBeanList.add(edged);
				}
				if (elementName.equals("activity")) {
					eList_pre = policyDAO.getSemanticActivity();
					eList_post = generateFileSerStrNode();
					// 修正节点显示的字符串 不用修正则直接创建节点
					for (String element : eList_pre) {
						dList_pre.add(new NodeBean(element));
					}
					for (String element : eList_post) {
						dList_post.add(new NodeBean(element));
					}
					// 节点生成完毕后 获得继承关系
					hashMap = policyDAO.getActivityMap();

					List<Map.Entry<String, String>> mapList = null;
					mapList = new ArrayList<Map.Entry<String, String>>(hashMap
							.entrySet());

					for (Map.Entry<String, String> elementEntry : mapList) {
						String str = elementEntry.getKey().substring(
								elementEntry.getKey().indexOf(':') + 1);

						NodeBean end = dList_pre.get(eList_pre.indexOf(str));
						NodeBean start = dList_post.get(eList_post
								.indexOf(elementEntry.getValue()));
						EdgeBean edge = new EdgeBean(start, end, new Long(20));
						edgeBeanList.add(edge);
					}
				}

			}

			// 解析数据，构造图
			gefNodeMap = new HashMap();
			graphNodeMap = new HashMap();
			edgeList = new ArrayList();
			directedGraph = new DirectedGraph();
			// GraphModel model = new DefaultGraphModel();
			graph.setModel(model);
			Map attributes = new Hashtable();
			// Set Arrow
			Map edgeAttrib = new Hashtable();
			GraphConstants.setLineEnd(edgeAttrib, GraphConstants.ARROW_CLASSIC);
			GraphConstants.setEndFill(edgeAttrib, true);
			graph.setJumpToDefaultPort(true);

			if (edgeBeanList == null || edgeBeanList.size() == 0) {
				graph.repaint();
				return null;
			}
			Iterator edgeBeanIt = edgeBeanList.iterator();
			while (edgeBeanIt.hasNext()) {
				EdgeBean edgeBean = (EdgeBean) edgeBeanIt.next();
				NodeBean sourceAction = edgeBean.getsourceNodeBean();
				NodeBean targetAction = edgeBean.gettargetNodeBean();
				// Long ageLong = edgeBean.getStatCount();
				// String edgeString = "(" + ageLong + ")";//edge 字符串
				String edgeString = "";
				addEdge(sourceAction, targetAction, 20, edgeString);
			}

			try {
				new DirectedGraphLayout().visit(directedGraph);
			} catch (Exception e1) {
				new NodeJoiningDirectedGraphLayout().visit(directedGraph);
			}

			int self_x = 50;
			int self_y = 50;
			int base_y = 10;
			if (graphNodeMap != null && gefNodeMap != null
					&& graphNodeMap.size() > gefNodeMap.size()) {
				base_y = self_y + GraphConstant.NODE_HEIGHT;
			}

			// 向图中添加节点node
			Collection nodeCollection = graphNodeMap.values();
			if (nodeCollection != null) {
				Iterator nodeIterator = nodeCollection.iterator();
				if (nodeIterator != null) {
					while (nodeIterator.hasNext()) {
						DefaultGraphCell node = (DefaultGraphCell) nodeIterator
								.next();
						NodeBean userObject = (NodeBean) node.getUserObject();
						if (userObject == null) {
							continue;
						}
						Node gefNode = (Node) gefNodeMap.get(userObject);
						if (gefNode == null) {
							gefNode = new Node();
							gefNode.x = self_x;
							gefNode.y = self_y - base_y;
							self_x += (10 + GraphConstant.NODE_WIDTH);
						}
						Map nodeAttrib = new Hashtable();
						GraphConstants.setBorderColor(nodeAttrib, Color.black);
						Rectangle2D Bounds = new Rectangle2D.Double(gefNode.x,
								gefNode.y + base_y, GraphConstant.NODE_WIDTH,
								GraphConstant.NODE_HEIGHT);
						GraphConstants.setBounds(nodeAttrib, Bounds);
						attributes.put(node, nodeAttrib);
					}// while
				}
			}

			// 向图中添加边
			if (edgeList == null) {
				// logger.error("edge list is null");
				return null;
			}
			for (int i = 0; i < edgeList.size(); i++) {
				UnionEdge unionEdge = (UnionEdge) edgeList.get(i);
				if (unionEdge == null) {
					// logger.error("union edge is null");
					continue;
				}
				ConnectionSet cs = new ConnectionSet(unionEdge.getEdge(),
						unionEdge.getSourceNode().getChildAt(0), unionEdge
								.getTargetNode().getChildAt(0));
				Object[] cells = new Object[] { unionEdge.getEdge(),
						unionEdge.getSourceNode(), unionEdge.getTargetNode() };
				attributes.put(unionEdge.getEdge(), edgeAttrib);
				model.insert(cells, attributes, cs, null, null);
			}

			graph.repaint();

		} catch (Exception e) {
			e.printStackTrace();
		}
		return graph;
	}

	private List<String> generateFileSerStrNode() {
		List<String> fileSerList = new ArrayList<String>();
		// String domainNames[] = service.getDomainNames();
		fileSerList.add("FileServices");

		return fileSerList;
	}

	private List<String> generateDomainStrNode() {
		List<String> domainList = new ArrayList<String>();
		String domainNames[] = service.getDomainNames();
		for (int i = 0; i < domainNames.length; i++) {
			domainList.add(domainNames[i]);
		}
		return domainList;
	}

	/**
	 * @param source
	 * @param target
	 */
	private void addEdge(NodeBean source, NodeBean target, int weight,
			String edgeString) {

		if (source == null || target == null) {
			return;
		}
		if (gefNodeMap == null) {
			gefNodeMap = new HashMap();
		}
		if (graphNodeMap == null) {
			graphNodeMap = new HashMap();
		}
		if (edgeList == null) {
			edgeList = new ArrayList();
		}
		if (directedGraph == null) {
			directedGraph = new DirectedGraph();
		}

		// 建立GEF的 node edge将来用来计算graph node的layout
		addEdgeGef(source, target, weight, edgeString);

		// 建立真正要用的graph的 node edge
		DefaultGraphCell sourceNode = null;
		DefaultGraphCell targetNode = null;
		sourceNode = (DefaultGraphCell) graphNodeMap.get(source);
		if (sourceNode == null) {
			sourceNode = new DefaultGraphCell(source);
			sourceNode.addPort();
			graphNodeMap.put(source, sourceNode);
		}
		targetNode = (DefaultGraphCell) graphNodeMap.get(target);
		if (targetNode == null) {
			targetNode = new DefaultGraphCell(target);
			targetNode.addPort();
			graphNodeMap.put(target, targetNode);
		}
		DefaultEdge edge = new DefaultEdge(edgeString);
		UnionEdge unionEdge = new UnionEdge();
		unionEdge.setEdge(edge);
		unionEdge.setSourceNode(sourceNode);
		unionEdge.setTargetNode(targetNode);

		edgeList.add(unionEdge);

	}

	/**
	 * @param source
	 * @param target
	 * @param weight
	 * @param edgeString
	 */
	private void addEdgeGef(NodeBean source, NodeBean target, int weight,
			String edgeString) {

		if (source.equals(target)) {
			return;
		}
		// 建立GEF的 node edge将来用来计算graph node的layout
		Node gefSourceNode = null;
		Node gefTargetNode = null;
		gefSourceNode = (Node) gefNodeMap.get(source);
		if (gefSourceNode == null) {
			gefSourceNode = new Node();
			gefSourceNode.width = GraphConstant.NODE_WIDTH;
			gefSourceNode.height = GraphConstant.NODE_WIDTH;
			gefSourceNode.setPadding(new Insets(GraphConstant.NODE_TOP_PAD,
					GraphConstant.NODE_LEFT_PAD, GraphConstant.NODE_BOTTOM_PAD,
					GraphConstant.NODE_RIGHT_PAD));
			directedGraph.nodes.add(gefSourceNode);
			gefNodeMap.put(source, gefSourceNode);
		}

		gefTargetNode = (Node) gefNodeMap.get(target);
		if (gefTargetNode == null) {
			gefTargetNode = new Node();
			gefTargetNode.width = GraphConstant.NODE_WIDTH;
			gefTargetNode.height = GraphConstant.NODE_WIDTH;
			gefTargetNode.setPadding(new Insets(GraphConstant.NODE_TOP_PAD,
					GraphConstant.NODE_LEFT_PAD, GraphConstant.NODE_BOTTOM_PAD,
					GraphConstant.NODE_RIGHT_PAD));
			directedGraph.nodes.add(gefTargetNode);
			gefNodeMap.put(target, gefTargetNode);
		}

		Edge gefEdge1 = null;
		try {
			gefEdge1 = new Edge(gefSourceNode, gefTargetNode);
			gefEdge1.weight = weight;
			directedGraph.edges.add(gefEdge1);
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

}
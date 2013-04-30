package cnd.conflict.detect;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;

import cnd.conflict.dao.PolicyDAO;

/**
 * @author YC
 * 
 */
public class ElementRelation {

	/**
	 * @param args
	 */
	private PolicyDAO policyDAO = new PolicyDAO();
	private static final int WUGUAN = 0; // 无关
	private static final int XIANGJIAO = 1;// 相交
	private static final int BAOHAN = 2;// 包含
	private static final int BAOHAN1 = 3;// 包含
	private static final int BAOHAN2 = 4;// 包含

	
	/**
	 * 
	 * @return
	 */
	public HashMap<String, List<String>> build_RoleMap(String dataBase) {
		HashMap<String, List<String>> map_Relation = new HashMap<String, List<String>>();
		HashMap<String, String> hashMap = policyDAO.getRoleMap();
		List<Map.Entry<String, String>> mapList = null;
		mapList = new ArrayList<Map.Entry<String, String>>(hashMap.entrySet());
		for (Map.Entry<String, String> element : mapList) {
			// System.out.println(element.getKey()+" "+element.getValue());
			//截取的子串str是子节点， element.getValue()是父节点
			int index = element.getValue().indexOf("\\");
			String domainName = element.getValue().substring(0, index);
			String str = domainName + "\\" + element.getKey().substring(
					element.getKey().indexOf(':') + 1);
            
			Set<String> set = map_Relation.keySet();

			if (set.contains(element.getValue())) {
				map_Relation.get(element.getValue()).add(str);
			} else {
				List<String> slist = new ArrayList<String>();
				slist.add(str);
				map_Relation.put(element.getValue(), slist);
			}
		}
		Set<String> set = map_Relation.keySet();
		for (String element : set) {
			List<String> sList = map_Relation.get(element);
			while (true) {
				List<String> sListTemp = new ArrayList<String>();
				for (String elementList : sList) {
					for (String elementSet : set) {
						if (elementList.equals(elementSet)) {
							for (String str : map_Relation.get(elementSet)) {
								// System.out.println(str);
								if (!sList.contains(str)) {
									sListTemp.add(str);
								}
								// System.out.println(sList);
							}

						}
					}
				}
				if (sListTemp.size() != 0) {

					for (String str : sListTemp) {
						if (!sList.contains(str)) {
							sList.add(str);
						}
					}
				}
				if (sListTemp.size() == 0) {
					break;
				}
			}
		}
		return map_Relation;
	}
	
	public HashMap<String, List<String>> build_RoleMap() {
		// TODO 构建Role关系图
		HashMap<String, List<String>> map_Relation = new HashMap<String, List<String>>();
		HashMap<String, String> hashMap = policyDAO.getRoleMap();
		List<Map.Entry<String, String>> mapList = null;
		mapList = new ArrayList<Map.Entry<String, String>>(hashMap.entrySet());
		for (Map.Entry<String, String> element : mapList) {
			// System.out.println(element.getKey()+" "+element.getValue());
		
			String str = element.getKey().substring(
					element.getKey().indexOf(':') + 1);

			Set<String> set = map_Relation.keySet();

			if (set.contains(str)) {
				map_Relation.get(str).add(element.getValue());
			} else {
				List<String> slist = new ArrayList<String>();
				slist.add(element.getValue());
				map_Relation.put(str, slist);
			}
		}
		/*
		 * List<Map.Entry<String, List<String>>> mapListTest = null; mapListTest
		 * = new ArrayList<Map.Entry<String,
		 * List<String>>>(map_Relation.entrySet());
		 * for(Map.Entry<String,List<String>> element:mapListTest){
		 * System.out.println(element.getKey()+" "+element.getValue()); }
		 */
		Set<String> set = map_Relation.keySet();
		for (String element : set) {
			List<String> sList = map_Relation.get(element);
			while (true) {
				List<String> sListTemp = new ArrayList<String>();
				for (String elementList : sList) {
					for (String elementSet : set) {
						if (elementList.equals(elementSet)) {
							for (String str : map_Relation.get(elementSet)) {
								// System.out.println(str);
								if (!sList.contains(str)) {
									sListTemp.add(str);
								}
								// System.out.println(sList);
							}

						}
					}
				}
				if (sListTemp.size() != 0) {

					for (String str : sListTemp) {
						if (!sList.contains(str)) {
							sList.add(str);
						}
					}
				}
				if (sListTemp.size() == 0) {
					break;
				}
			}
		}
		return map_Relation;
	}

	/**
	 * 
	 * @param node1
	 * @param node2
	 * @return
	 */
	//可以出现多种情况 build_ActivityMap可以给出
	public int judge_Relation_Activity(String node1, String node2,
			String dataBase) {
		ConflictAnalysis conflictAnalysis = ConflictAnalysis.getInstance();
		HashMap<String, List<String>> map_Relation = build_ActivityMap(dataBase);
		Set<String> set = map_Relation.keySet();
		List<String> slist1 = map_Relation.get(node1);
		List<String> slist2 = map_Relation.get(node2);
		if (node1.equals(node2)) {
			conflictAnalysis.setFileActPos(node1);
			conflictAnalysis.setFileActInc(2);
			return BAOHAN;
		}
		// 为空说明没有子节点
		if (slist1 == null && slist2 == null) {
			return WUGUAN;
		} else if (slist1 == null) {
			if (slist2.contains(node1)) {
				conflictAnalysis.setFileActPos(node1);
				conflictAnalysis.setFileActInc(1);
				return BAOHAN2;
			} else
				return WUGUAN;
		} else if (slist2 == null) {
			if (slist1.contains(node2)) {
				conflictAnalysis.setFileActPos(node2);
				conflictAnalysis.setFileActInc(0);
				return BAOHAN1;
			} else
				return WUGUAN;
		} else {
			
			if (slist1.contains(node2)) {
				conflictAnalysis.setFileActPos(node2);
				conflictAnalysis.setFileActInc(0);
				return BAOHAN1;
			}
			if (slist2.contains(node1)) {
				conflictAnalysis.setFileActPos(node1);
				conflictAnalysis.setFileActInc(1);
				return BAOHAN2;
			}
			for (String element : slist1) {
				if (slist2.contains(element)) {
					conflictAnalysis.setFileActPos(element);
					conflictAnalysis.setFileActInc(3);
					return XIANGJIAO;
				}
			}
			return WUGUAN;
		}
	}
	public HashMap<String, List<String>> build_ActivityMap(String dataBase) {
		HashMap<String, List<String>> map_Relation = new HashMap<String, List<String>>();
		String str = "RWX";
		List<String> slist1 = new ArrayList<String>();
		slist1.add("RW");
		slist1.add("RX");
		slist1.add("WX");
		slist1.add("R");
		slist1.add("W");
		slist1.add("X");
		map_Relation.put(str, slist1);
		str = "RW";
		List<String> slist2 = new ArrayList<String>();
		slist2.add("R");
		slist2.add("W");
		map_Relation.put(str, slist2);
		str = "RX";
		List<String> slist3 = new ArrayList<String>();
		slist3.add("R");
		slist3.add("X");
		map_Relation.put(str, slist3);
		str = "WX";
		List<String> slist4 = new ArrayList<String>();
		slist4.add("W");
		slist4.add("X");
		map_Relation.put(str, slist4);
		return map_Relation;
	}

	//判断是无关还是包含的关系
	public int judge_Relation_Role(String node1, String node2,
			String dataBase) {
		ConflictAnalysis conflictAnalysis = ConflictAnalysis.getInstance();
		HashMap<String, List<String>> map_Relation = build_RoleMap(dataBase);
		Set<String> set = map_Relation.keySet();
		List<String> slist1 = map_Relation.get(node1);
		List<String> slist2 = map_Relation.get(node2);
		if (node1.equals(node2)) {
			conflictAnalysis.setFileRoleInc(2);
			return BAOHAN;
		}
		// 为空说明没有子节点
		if (slist1 == null && slist2 == null) {
			return WUGUAN;
		} else if (slist1 == null) {
			if (slist2.contains(node1)) {
				conflictAnalysis.setFileRoleInc(1);
				return BAOHAN2;
			} else
				return WUGUAN;
		} else if (slist2 == null) {
			if (slist1.contains(node2)) {
				conflictAnalysis.setFileRoleInc(0);
				return BAOHAN1;
			} else
				return WUGUAN;
		} else {
			
			if (slist1.contains(node2)) {
				conflictAnalysis.setFileRoleInc(0);
				return BAOHAN1;
			}
			if (slist2.contains(node1)) {
				conflictAnalysis.setFileRoleInc(1);
				return BAOHAN2;
			}
			for (String element : slist1) {
				if (slist2.contains(element)) {
					conflictAnalysis.setFileRoleInc(3);
					return XIANGJIAO;
				}
			}
			return WUGUAN;
		}
	}
	
	

	public int judge_Relation_Role(String node1, String node2) {
		// TODO 判断Role元组关系
		HashMap<String, List<String>> map_Relation = build_RoleMap();
		Set<String> set = map_Relation.keySet();
		List<String> slist1 = map_Relation.get(node1);
		List<String> slist2 = map_Relation.get(node2);
		if (node1.equals(node2)) {
			return BAOHAN;
		}
		// 为空说明没有子节点
		if (slist1 == null && slist2 == null) {
			return WUGUAN;
		} else if (slist1 == null) {
			if (slist2.contains(node1)) {
				return BAOHAN2;
			} else
				return WUGUAN;
		} else if (slist2 == null) {
			if (slist1.contains(node2)) {
				return BAOHAN1;
			} else
				return WUGUAN;
		} else {
			/*
			 * if(slist1.contains(node2) || slist2.contains(node1)){ return
			 * BAOHAN; }else{ for(String element:slist1){
			 * if(slist2.contains(element)){ return XIANGJIAO; } } return
			 * WUGUAN; }
			 */
			if (slist1.contains(node2)) {
				return BAOHAN1;
			}
			if (slist2.contains(node1)) {
				return BAOHAN2;
			}
			for (String element : slist1) {
				if (slist2.contains(element)) {
					return XIANGJIAO;
				}
			}
			return WUGUAN;
		}
	}

	/**
	 * 
	 * @return
	 */
	public HashMap<String, List<String>> build_ContextMap() {
		// TODO 构建Context关系图
		HashMap<String, List<String>> map_Relation = new HashMap<String, List<String>>();
		HashMap<String, String> hashMap = policyDAO.getContextMap();
		List<Map.Entry<String, String>> mapList = null;
		mapList = new ArrayList<Map.Entry<String, String>>(hashMap.entrySet());
		for (Map.Entry<String, String> element : mapList) {
			// System.out.println(element.getKey()+" "+element.getValue());
			String str = element.getKey().substring(
					element.getKey().indexOf('_') + 1);
			Set<String> set = map_Relation.keySet();
			if (set.contains(str)) {
				map_Relation.get(str).add(element.getValue());
			} else {
				List<String> slist = new ArrayList<String>();
				slist.add(element.getValue());
				map_Relation.put(str, slist);
			}
		}
		/*
		 * List<Map.Entry<String, List<String>>> mapListTest = null; mapListTest
		 * = new ArrayList<Map.Entry<String,
		 * List<String>>>(map_Relation.entrySet());
		 * for(Map.Entry<String,List<String>> element:mapListTest){
		 * System.out.println(element.getKey()+" "+element.getValue()); }
		 */
		Set<String> set = map_Relation.keySet();
		for (String element : set) {
			List<String> sList = map_Relation.get(element);
			while (true) {
				List<String> sListTemp = new ArrayList<String>();
				for (String elementList : sList) {
					for (String elementSet : set) {
						if (elementList.equals(elementSet)) {
							for (String str : map_Relation.get(elementSet)) {
								// System.out.println(str);
								if (!sList.contains(str)) {
									sListTemp.add(str);
								}
								// System.out.println(sList);
							}

						}
					}
				}
				if (sListTemp.size() != 0) {

					for (String str : sListTemp) {
						if (!sList.contains(str)) {
							sList.add(str);
						}
					}
				}
				if (sListTemp.size() == 0) {
					break;
				}
			}
		}
		return map_Relation;
	}

	public int judge_Relation_Organization(String node1, String node2,
			String dataBase) {
		if (node1.equals(node2)) {
			return BAOHAN;
		} else {
			return WUGUAN;
		}
	}

	/**
	 * 
	 * @param node1
	 * @param node2
	 * @return
	 */
	public int judge_Relation_Context(String node1, String node2,
			String dataBase) {
		if (node1.equals(node2)) {
			return BAOHAN;
		} else {
			return WUGUAN;
		}
	}
	
	public int judge_Relation_View(String node1, String node2,
			String dataBase) {
		if (node1.equals(node2)) {
			return BAOHAN;
		} else {
			return WUGUAN;
		}
	}
	

	
	
	

	public int judge_Relation_Context(String node1, String node2) {
		// TODO 判断Context元组关系
		HashMap<String, List<String>> map_Relation = build_ContextMap();
		Set<String> set = map_Relation.keySet();
		List<String> slist1 = map_Relation.get(node1);
		List<String> slist2 = map_Relation.get(node2);
		if (node1.equals(node2)) {
			return BAOHAN;
		}
		if (slist1 == null && slist2 == null) {
			return WUGUAN;
		} else if (slist1 == null) {
			if (slist2.contains(node1)) {
				return BAOHAN2;
			} else
				return WUGUAN;
		} else if (slist2 == null) {
			if (slist1.contains(node2)) {
				return BAOHAN1;
			} else
				return WUGUAN;
		} else {
			/*
			 * if(slist1.contains(node2) || slist2.contains(node1)){ return
			 * BAOHAN; }else{ for(String element:slist1){
			 * if(slist2.contains(element)){ return XIANGJIAO; } } return
			 * WUGUAN; }
			 */
			if (slist1.contains(node2)) {
				return BAOHAN1;
			}
			if (slist2.contains(node1)) {
				return BAOHAN2;
			}
			for (String element : slist1) {
				if (slist2.contains(element)) {
					return XIANGJIAO;
				}
			}
			return WUGUAN;
		}
	}

	/**
	 * 
	 * @return
	 */
	public HashMap<String, List<String>> build_ViewMap() {
		// TODO 构建View关系图
		HashMap<String, List<String>> map_Relation = new HashMap<String, List<String>>();
		HashMap<String, String> hashMap = policyDAO.getViewMap();
		List<Map.Entry<String, String>> mapList = null;
		mapList = new ArrayList<Map.Entry<String, String>>(hashMap.entrySet());
		for (Map.Entry<String, String> element : mapList) {
			// System.out.println(element.getKey()+" "+element.getValue());
			String str = element.getKey().substring(
					element.getKey().indexOf('_') + 1);
			Set<String> set = map_Relation.keySet();
			if (set.contains(str)) {
				map_Relation.get(str).add(element.getValue());
			} else {
				List<String> slist = new ArrayList<String>();
				slist.add(element.getValue());
				map_Relation.put(str, slist);
			}
		}
		/*
		 * List<Map.Entry<String, List<String>>> mapListTest = null; mapListTest
		 * = new ArrayList<Map.Entry<String,
		 * List<String>>>(map_Relation.entrySet());
		 * for(Map.Entry<String,List<String>> element:mapListTest){
		 * System.out.println(element.getKey()+" "+element.getValue()); }
		 */
		Set<String> set = map_Relation.keySet();
		for (String element : set) {
			List<String> sList = map_Relation.get(element);
			while (true) {
				List<String> sListTemp = new ArrayList<String>();
				for (String elementList : sList) {
					for (String elementSet : set) {
						if (elementList.equals(elementSet)) {
							for (String str : map_Relation.get(elementSet)) {
								// System.out.println(str);
								if (!sList.contains(str)) {
									sListTemp.add(str);
								}
								// System.out.println(sList);
							}

						}
					}
				}
				if (sListTemp.size() != 0) {

					for (String str : sListTemp) {
						if (!sList.contains(str)) {
							sList.add(str);
						}
					}
				}
				if (sListTemp.size() == 0) {
					break;
				}
			}
		}
		return map_Relation;
	}

	/**
	 * 
	 * @param node1
	 * @param node2
	 * @return
	 */
	public int judge_Relation_View(String node1, String node2) {
		// TODO 判断View元组关系
		HashMap<String, List<String>> map_Relation = build_ViewMap();
		Set<String> set = map_Relation.keySet();
		List<String> slist1 = map_Relation.get(node1);
		List<String> slist2 = map_Relation.get(node2);
		if (node1.equals(node2)) {
			return BAOHAN;
		}
		if (slist1 == null && slist2 == null) {
			return WUGUAN;
		} else if (slist1 == null) {
			if (slist2.contains(node1)) {
				return BAOHAN2;
			} else
				return WUGUAN;
		} else if (slist2 == null) {
			if (slist1.contains(node2)) {
				return BAOHAN1;
			} else
				return WUGUAN;
		} else {
			/*
			 * if(slist1.contains(node2) || slist2.contains(node1)){ return
			 * BAOHAN; }else{ for(String element:slist1){
			 * if(slist2.contains(element)){ return XIANGJIAO; } } return
			 * WUGUAN; }
			 */
			if (slist1.contains(node2)) {
				return BAOHAN1;
			}
			if (slist2.contains(node1)) {
				return BAOHAN2;
			}
			for (String element : slist1) {
				if (slist2.contains(element)) {
					return XIANGJIAO;
				}
			}
			return WUGUAN;
		}
	}

	/**
	 * 
	 * @return
	 */
	public HashMap<String, List<String>> build_ActivityMap() {
		// TODO 构建Activity关系图
		HashMap<String, List<String>> map_Relation = new HashMap<String, List<String>>();
		HashMap<String, String> hashMap = policyDAO.getActivityMap();
		List<Map.Entry<String, String>> mapList = null;
		mapList = new ArrayList<Map.Entry<String, String>>(hashMap.entrySet());
		for (Map.Entry<String, String> element : mapList) {
			// System.out.println(element.getKey()+" "+element.getValue());
			String str = element.getKey().substring(
					element.getKey().indexOf('_') + 1);
			Set<String> set = map_Relation.keySet();
			if (set.contains(str)) {
				map_Relation.get(str).add(element.getValue());
			} else {
				List<String> slist = new ArrayList<String>();
				slist.add(element.getValue());
				map_Relation.put(str, slist);
			}
		}
		/*
		 * List<Map.Entry<String, List<String>>> mapListTest = null; mapListTest
		 * = new ArrayList<Map.Entry<String,
		 * List<String>>>(map_Relation.entrySet());
		 * for(Map.Entry<String,List<String>> element:mapListTest){
		 * System.out.println(element.getKey()+" "+element.getValue()); }
		 */
		Set<String> set = map_Relation.keySet();
		for (String element : set) {
			List<String> sList = map_Relation.get(element);
			while (true) {
				List<String> sListTemp = new ArrayList<String>();
				for (String elementList : sList) {
					for (String elementSet : set) {
						if (elementList.equals(elementSet)) {
							for (String str : map_Relation.get(elementSet)) {
								// System.out.println(str);
								if (!sList.contains(str)) {
									sListTemp.add(str);
								}
								// System.out.println(sList);
							}

						}
					}
				}
				if (sListTemp.size() != 0) {

					for (String str : sListTemp) {
						if (!sList.contains(str)) {
							sList.add(str);
						}
					}
				}
				if (sListTemp.size() == 0) {
					break;
				}
			}
		}
		return map_Relation;
	}

	/**
	 * 
	 * @param node1
	 * @param node2
	 * @return
	 */
	public int judge_Relation_Activity(String node1, String node2) {
		// TODO 判断Activity元组关系
		HashMap<String, List<String>> map_Relation = build_ActivityMap();
		Set<String> set = map_Relation.keySet();
		List<String> slist1 = map_Relation.get(node1);
		List<String> slist2 = map_Relation.get(node2);
		if (node1.equals(node2)) {
			return BAOHAN;
		}
		if (slist1 == null && slist2 == null) {
			return WUGUAN;
		} else if (slist1 == null) {
			if (slist2.contains(node1)) {
				return BAOHAN2;
			} else
				return WUGUAN;
		} else if (slist2 == null) {
			if (slist1.contains(node2)) {
				return BAOHAN1;
			} else
				return WUGUAN;
		} else {
			/*
			 * if(slist1.contains(node2) || slist2.contains(node1)){ return
			 * BAOHAN; }else{ for(String element:slist1){
			 * if(slist2.contains(element)){ return XIANGJIAO; } } return
			 * WUGUAN; }
			 */
			if (slist1.contains(node2)) {
				return BAOHAN1;
			}
			if (slist2.contains(node1)) {
				return BAOHAN2;
			}
			for (String element : slist1) {
				if (slist2.contains(element)) {
					return XIANGJIAO;
				}
			}
			return WUGUAN;
		}
	}

/*	public static void main(String[] args) {
		// TODO Auto-generated method stub

		List<Map.Entry<String, List<String>>> mapList = null;

		HashMap<String, List<String>> map_Relation = new ElementRelation()
				.build_RoleMap();
		mapList = new ArrayList<Map.Entry<String, List<String>>>(map_Relation
				.entrySet());
		for (Map.Entry<String, List<String>> element : mapList) {
			System.out.println(element.getKey() + " " + element.getValue());
		}
		System.out.println(new ElementRelation().judge_Relation_Role("Manager",
				"Manager"));
		System.out.println(new ElementRelation().judge_Relation_Role(
				"Projecter", "SoftWare_Engineer"));
		System.out.println(new ElementRelation().judge_Relation_Role("UserB",
				"SoftWare_Engineer"));
		System.out.println(new ElementRelation().judge_Relation_Role("UserB",
				"UserC"));
		System.out.println(new ElementRelation().judge_Relation_Role(
				"QA_Engineer", "SoftWare_Engineer"));

		// new ElementRelation().build_RoleMap();

	}*/

}

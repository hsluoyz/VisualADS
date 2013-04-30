package cnd.conflict.dao;

import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.List;
import java.util.Map;
import java.util.Set;

import cnd.conflict.entity.CndPolicy;
import cnd.conflict.entity.Result;
import cnd.conflict.util.DataBaseConn;

/**
 * 
 * @author YC
 * 
 */
public class PolicyDAO {

	// private static PolicyDAO policyDAO = null;
	private DataBaseConn dbc = DataBaseConn.getDataBaseConnInstance();

	// 查询Role信息
	public List<String> getRoleElementParentList(String element) {
		String sql = "select * from role where elementName = ? ";
		PreparedStatement psmt = null;

		List<String> elementParentList = new ArrayList<String>();
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			psmt.setString(1, element);
			ResultSet rs = psmt.executeQuery();
			while (rs.next()) {
				elementParentList.add(rs.getString(3));
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		// logger.info("get data");
		return elementParentList;
	}

	// 查询role map
	public HashMap<String, String> getRoleMap() {
		String sql = "select * from role";
		PreparedStatement psmt = null;
		HashMap<String, String> roleMap = new HashMap<String, String>();
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			ResultSet rs = psmt.executeQuery();
			while (rs.next()) {
				roleMap.put(rs.getString(3) + ":" + rs.getString(2), rs
						.getString(3));
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return roleMap;
	}

	// 查询View信息
	public List<String> getViewElementParentList(String element) {
		String sql = "select * from view where elementName = ? ";
		PreparedStatement psmt = null;

		List<String> elementParentList = new ArrayList<String>();
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			psmt.setString(1, element);
			ResultSet rs = psmt.executeQuery();
			while (rs.next()) {
				elementParentList.add(rs.getString(3));
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		// logger.info("get data");
		return elementParentList;
	}

	// 查询view map
	public HashMap<String, String> getViewMap() {
		String sql = "select * from view";
		PreparedStatement psmt = null;
		HashMap<String, String> roleMap = new HashMap<String, String>();
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			ResultSet rs = psmt.executeQuery();
			while (rs.next()) {
				roleMap.put(rs.getString(3) + ":" + rs.getString(2), rs
						.getString(3));
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return roleMap;
	}

	// 查询Context信息
	public List<String> getContextElementParentList(String element) {
		String sql = "select * from context where elementName = ? ";
		PreparedStatement psmt = null;

		List<String> elementParentList = new ArrayList<String>();
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			psmt.setString(1, element);
			ResultSet rs = psmt.executeQuery();
			while (rs.next()) {
				elementParentList.add(rs.getString(3));
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		// logger.info("get data");
		return elementParentList;
	}

	// 查询context map
	public HashMap<String, String> getContextMap() {
		String sql = "select * from context";
		PreparedStatement psmt = null;
		HashMap<String, String> roleMap = new HashMap<String, String>();
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			ResultSet rs = psmt.executeQuery();
			while (rs.next()) {
				// 加上“_”为保证Map的KEY是唯一的 样例数据库要的是后面的字串 所以可以这么改
				roleMap.put(rs.getString(3) + ":" + rs.getString(2), rs
						.getString(3));
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return roleMap;
	}

	// 查询Activity信息
	public List<String> getActivityElementParentList(String element) {
		String sql = "select * from activity where elementName = ? ";
		PreparedStatement psmt = null;

		List<String> elementParentList = new ArrayList<String>();
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			psmt.setString(1, element);
			ResultSet rs = psmt.executeQuery();
			while (rs.next()) {
				elementParentList.add(rs.getString(3));
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		// logger.info("get data");
		return elementParentList;
	}

	// 查询activity map
	public HashMap<String, String> getActivityMap() {
		String sql = "select * from activity";
		PreparedStatement psmt = null;
		HashMap<String, String> roleMap = new HashMap<String, String>();
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			ResultSet rs = psmt.executeQuery();
			while (rs.next()) {
				roleMap.put(rs.getString(3) + ":" + rs.getString(2), rs
						.getString(3));
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return roleMap;
	}

	// 查询Measure信息
	public List<String> getMeasureElementDenyList(String element) {
		String sql = "select * from measure where elementName = ? ";
		PreparedStatement psmt = null;

		List<String> elementDenyList = new ArrayList<String>();
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			psmt.setString(1, element);
			ResultSet rs = psmt.executeQuery();
			while (rs.next()) {
				elementDenyList.add(rs.getString(3));
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		// logger.info("get data");
		return elementDenyList;
	}

	/*
	 * // 获取role elementName public List<String> getRole() { String sql =
	 * "select * from role "; PreparedStatement psmt = null;
	 * 
	 * List<String> sList = new ArrayList<String>(); try { psmt =
	 * dbc.getConn().prepareStatement(sql); ResultSet rs = psmt.executeQuery();
	 * while (rs.next()) { sList.add(rs.getString(2)); } rs.close();
	 * psmt.close(); } catch (Exception e) { e.printStackTrace(); } return
	 * sList; }
	 * 
	 * public List<String> getContext() { String sql = "select * from context ";
	 * PreparedStatement psmt = null;
	 * 
	 * List<String> sList = new ArrayList<String>(); try { psmt =
	 * dbc.getConn().prepareStatement(sql); ResultSet rs = psmt.executeQuery();
	 * while (rs.next()) { sList.add(rs.getString(2)); } rs.close();
	 * psmt.close(); } catch (Exception e) { e.printStackTrace(); } return
	 * sList; }
	 */
	/*
	 * // 获得所有context表中的元素 public Set<String> getAllContextEle() { String sql =
	 * "select * from context "; PreparedStatement psmt = null;
	 * 
	 * Set<String> eleSet = new HashSet<String>(); try { psmt =
	 * dbc.getConn().prepareStatement(sql); ResultSet rs = psmt.executeQuery();
	 * while (rs.next()) { eleSet.add(rs.getString(2));
	 * eleSet.add(rs.getString(3)); } rs.close(); psmt.close(); } catch
	 * (Exception e) { e.printStackTrace(); }
	 * 
	 * return eleSet; }
	 * 
	 * public List<String> getActivity() { String sql =
	 * "select * from activity "; PreparedStatement psmt = null;
	 * 
	 * List<String> sList = new ArrayList<String>(); try { psmt =
	 * dbc.getConn().prepareStatement(sql); ResultSet rs = psmt.executeQuery();
	 * while (rs.next()) { sList.add(rs.getString(2)); } rs.close();
	 * psmt.close(); } catch (Exception e) { e.printStackTrace(); } return
	 * sList; }
	 */
	/*
	 * public List<String> getView() { String sql = "select * from view ";
	 * PreparedStatement psmt = null;
	 * 
	 * List<String> sList = new ArrayList<String>(); try { psmt =
	 * dbc.getConn().prepareStatement(sql); ResultSet rs = psmt.executeQuery();
	 * while (rs.next()) { sList.add(rs.getString(2)); } rs.close();
	 * psmt.close(); } catch (Exception e) { e.printStackTrace(); } return
	 * sList; }
	 */

	// 获取role semantic mapping element
	public List<String> getSemanticRole() {
		String sql = "select * from semantic_Role ";
		PreparedStatement psmt = null;

		List<String> sList = new ArrayList<String>();
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			ResultSet rs = psmt.executeQuery();
			while (rs.next()) {
				sList.add(rs.getString(2));
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return sList;
	}

	// 获取role semantic map
	public List<String> getSemanticRoleMap() {
		String sql = "select * from semantic_Role ";
		PreparedStatement psmt = null;

		List<String> sList = new ArrayList<String>();
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			ResultSet rs = psmt.executeQuery();
			while (rs.next()) {
				sList.add(rs.getString(2) + "<----->" + rs.getString(3));
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return sList;
	}

	// 获取Context semantic mapping
	public List<String> getSemanticContext() {
		String sql = "select * from semantic_Context ";
		PreparedStatement psmt = null;

		List<String> sList = new ArrayList<String>();
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			ResultSet rs = psmt.executeQuery();
			while (rs.next()) {
				sList.add(rs.getString(2));
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return sList;
	}

	// 获取view semantic mapping
	public List<String> getSemanticView() {
		String sql = "select * from semantic_View ";
		PreparedStatement psmt = null;

		List<String> sList = new ArrayList<String>();
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			ResultSet rs = psmt.executeQuery();
			while (rs.next()) {
				sList.add(rs.getString(2));
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return sList;
	}

	// 获取view semantic map
	public List<String> getSemanticViewMap() {
		String sql = "select * from semantic_View ";
		PreparedStatement psmt = null;

		List<String> sList = new ArrayList<String>();
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			ResultSet rs = psmt.executeQuery();
			while (rs.next()) {
				sList.add(rs.getString(2) + "<----->" + rs.getString(3));
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return sList;
	}

	// 获取Activity semantic mapping
	public List<String> getSemanticActivity() {
		String sql = "select * from semantic_Activity ";
		PreparedStatement psmt = null;

		List<String> sList = new ArrayList<String>();
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			ResultSet rs = psmt.executeQuery();
			while (rs.next()) {
				sList.add(rs.getString(2));
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return sList;
	}

	// 获取activity semantic map
	public List<String> getSemanticActivityMap() {
		String sql = "select * from semantic_Activity ";
		PreparedStatement psmt = null;

		List<String> sList = new ArrayList<String>();
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			ResultSet rs = psmt.executeQuery();
			while (rs.next()) {
				sList.add(rs.getString(2) + "<----->" + rs.getString(3));
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return sList;
	}

	// 获取Measure semantic mapping
	public List<String> getSemanticMeasure() {
		String sql = "select * from semantic_Measure ";
		PreparedStatement psmt = null;

		List<String> sList = new ArrayList<String>();
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			ResultSet rs = psmt.executeQuery();
			while (rs.next()) {
				sList.add(rs.getString(2));
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return sList;
	}

	// 获取Measure semantic mapping by type
	public List<String> getSemanticMeasureByType(int type) {
		String sql = "select * from semantic_Measure where type = ?";
		PreparedStatement psmt = null;
		List<String> sList = new ArrayList<String>();
		try {

			psmt = dbc.getConn().prepareStatement(sql);
			psmt.setInt(1, type);
			ResultSet rs = psmt.executeQuery();
			while (rs.next()) {
				sList.add(rs.getString(2));
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return sList;
	}

	// 获取策略列表
	public List<CndPolicy> getPolicy() {
		String sql = "select * from policy";
		PreparedStatement psmt = null;
		List<CndPolicy> policyList = new ArrayList<CndPolicy>();
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			ResultSet rs = psmt.executeQuery();
			while (rs.next()) {
				CndPolicy cndPolicy = new CndPolicy();
				cndPolicy.setPolicyId(rs.getInt(1));
				cndPolicy.setType(rs.getInt(2));
				cndPolicy.setContext(rs.getString(3));
				cndPolicy.setRole(rs.getString(4));
				cndPolicy.setView(rs.getString(5));
				cndPolicy.setActivity(rs.getString(6));
				cndPolicy.setMeasure(rs.getString(7));
				cndPolicy.setOrganization(rs.getString(8));
				policyList.add(cndPolicy);
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return policyList;
	}

	// 获取一条策略
	public CndPolicy getOnePolicy(int id) {
		CndPolicy cndPolicy = null;
		String sql = "select * from policy where id = ?";
		PreparedStatement psmt = null;
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			psmt.setInt(1, id);
			ResultSet rs = psmt.executeQuery();
			if (rs.next()) {
				cndPolicy = new CndPolicy();
				cndPolicy.setPolicyId(rs.getInt(1));
				cndPolicy.setType(rs.getInt(2));
				cndPolicy.setContext(rs.getString(3));
				cndPolicy.setRole(rs.getString(4));
				cndPolicy.setView(rs.getString(5));
				cndPolicy.setActivity(rs.getString(6));
				cndPolicy.setMeasure(rs.getString(7));
				cndPolicy.setOrganization(rs.getString(8));
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return cndPolicy;
	}

	// 查询element map
	public HashMap<Integer, String> getElementMap(int number) {
		String sql = "select * from policy";
		PreparedStatement psmt = null;
		HashMap<Integer, String> elementMap = new HashMap<Integer, String>();
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			ResultSet rs = psmt.executeQuery();
			while (rs.next()) {
				if (number == 3 && "fileservices" == dbc.getUrl_database()) {
					elementMap.put(rs.getInt(1), rs.getString(8) + "\\"
							+ rs.getString(number));
				} else if (number == 5
						&& "fileservices" == dbc.getUrl_database()) {
					elementMap.put(rs.getInt(1), rs.getString(8) + "\\"
							+ rs.getString(3) + "\\" + rs.getString(number));
				} else {
					elementMap.put(rs.getInt(1), rs.getString(number));
				}
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return elementMap;
	}

	// 查询type map
	public HashMap<Integer, Integer> getTypeMap(int number) {
		String sql = "select * from policy";
		PreparedStatement psmt = null;
		HashMap<Integer, Integer> elementMap = new HashMap<Integer, Integer>();
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			ResultSet rs = psmt.executeQuery();
			while (rs.next()) {
				elementMap.put(rs.getInt(1), rs.getInt(number));
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return elementMap;
	}

	// 获取总记录数
	public int getPolicyAmount() {
		int i = 0;
		String sql = "select count(*) from policy";
		PreparedStatement psmt = null;
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			ResultSet rs = psmt.executeQuery();
			if (rs.next()) {
				i = rs.getInt(1);
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return i;
	}

	// 删除信息
	public int delete(int id) {
		int i = 0;
		String sql = "delete from policy where id= ? ";
		PreparedStatement psmt = null;
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			psmt.setInt(1, id);
			i = psmt.executeUpdate();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return i;
	}

	// 添加新策略
	public int addPolicy(CndPolicy cndPolicy) {
		int i = 0;
		String sql = "insert into policy values (null,?,?,?,?,?,?,?)";
		PreparedStatement psmt = null;
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			psmt.setInt(1, cndPolicy.getType());
			psmt.setString(2, cndPolicy.getContext());
			psmt.setString(3, cndPolicy.getRole());
			psmt.setString(4, cndPolicy.getView());
			psmt.setString(5, cndPolicy.getActivity());
			psmt.setString(6, cndPolicy.getMeasure());
			psmt.setString(7, cndPolicy.getOrganization());

			i = psmt.executeUpdate();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return i;
	}

	// update policy
	public int updatePolicy(CndPolicy cndPolicy) {
		int i = 0;
		// String sql = "update policy set type = ?,context=?,role=?," +
		// "view=?,activity=?,measure=? where id=?";

		String sql = "update policy set type=?,context=?,role=?,"
				+ "view=?,activity=?,measure=? where id=?";
		PreparedStatement psmt = null;
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			psmt.setInt(1, cndPolicy.getType());
			psmt.setString(2, cndPolicy.getContext());
			psmt.setString(3, cndPolicy.getRole());
			psmt.setString(4, cndPolicy.getView());
			psmt.setString(5, cndPolicy.getActivity());
			psmt.setString(6, cndPolicy.getMeasure());
			psmt.setInt(7, cndPolicy.getPolicyId());
			i = psmt.executeUpdate();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return i;
	}

	// 添加result
	public int addResult(Result result) {
		int i = 0;
		String sql = "insert into result_conflict values (null,?,?,?,?,?,?)";
		PreparedStatement psmt = null;
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			psmt.setInt(1, result.getPolicyAmount());
			psmt.setInt(2, result.getConflictAmount());
			psmt.setInt(3, result.getUnrelation());
			psmt.setInt(4, result.getSyntaxconflict());
			psmt.setInt(5, result.getInclusivematch());
			psmt.setInt(6, result.getPartialmatch());
			i = psmt.executeUpdate();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return i;
	}

	// 获取result List
	public List<Result> getResultList() {
		String sql = "select * from result_conflict ";
		PreparedStatement psmt = null;
		List<Result> resultList = new ArrayList<Result>();
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			ResultSet rs = psmt.executeQuery();
			while (rs.next()) {
				Result result = new Result();
				result.setPolicyAmount(rs.getInt(2));
				result.setConflictAmount(rs.getInt(3));
				result.setUnrelation(rs.getInt(4));
				result.setSyntaxconflict(rs.getInt(5));
				result.setInclusivematch(rs.getInt(6));
				result.setPartialmatch(rs.getInt(7));
				resultList.add(result);
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return resultList;
	}

	// 获取policyAmount List
	public List<Integer> getPolicyAmountList() {
		String sql = "select * from result_conflict ";
		PreparedStatement psmt = null;
		List<Integer> policyAmountList = new ArrayList<Integer>();
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			ResultSet rs = psmt.executeQuery();
			while (rs.next()) {
				policyAmountList.add(rs.getInt(2));
			}
			rs.close();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return policyAmountList;
	}

	// 添加role
	public int addRole(String elementName, String elmentParent) {
		int i = 0;
		String sql = "insert into role values (null,?,?)";
		PreparedStatement psmt = null;
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			psmt.setString(1, elementName);
			psmt.setString(2, elmentParent);

			i = psmt.executeUpdate();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return i;
	}

	// 添加role semantic
	public int addRoleSemantic(String elementName, String elmentSemantic) {
		int i = 0;
		String sql = "insert into semantic_role values (null,?,?)";
		PreparedStatement psmt = null;
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			psmt.setString(1, elementName);
			psmt.setString(2, elmentSemantic);

			i = psmt.executeUpdate();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return i;
	}

	// 添加view
	public int addView(String elementName, String elmentParent) {
		int i = 0;
		String sql = "insert into view values (null,?,?)";
		PreparedStatement psmt = null;
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			psmt.setString(1, elementName);
			psmt.setString(2, elmentParent);
			i = psmt.executeUpdate();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return i;
	}

	// 添加view semantic
	public int addViewSemantic(String elementName, String elmentSemantic) {
		int i = 0;
		String sql = "insert into semantic_view values (null,?,?)";
		PreparedStatement psmt = null;
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			psmt.setString(1, elementName);
			psmt.setString(2, elmentSemantic);
			i = psmt.executeUpdate();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return i;
	}

	// 添加context
	public int addContext(String elementName, String elmentParent) {
		int i = 0;
		String sql = "insert into context values (null,?,?)";
		PreparedStatement psmt = null;
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			psmt.setString(1, elementName);
			psmt.setString(2, elmentParent);
			i = psmt.executeUpdate();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return i;
	}

	// 添加context semantic
	public int addContextSemantic(String elementName, String elmentSemantic) {
		int i = 0;
		String sql = "insert into semantic_context values (null,?,?)";
		PreparedStatement psmt = null;
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			psmt.setString(1, elementName);
			psmt.setString(2, elmentSemantic);
			i = psmt.executeUpdate();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return i;
	}

	// 添加Activity
	public int addActivity(String elementName, String elmentParent) {
		int i = 0;
		String sql = "insert into activity values (null,?,?)";
		PreparedStatement psmt = null;
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			psmt.setString(1, elementName);
			psmt.setString(2, elmentParent);

			i = psmt.executeUpdate();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return i;
	}

	// 添加Activity semantic
	public int addActivitySemantic(String elementName, String elmentSemantic) {
		int i = 0;
		String sql = "insert into semantic_activity values (null,?,?)";
		PreparedStatement psmt = null;
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			psmt.setString(1, elementName);
			psmt.setString(2, elmentSemantic);

			i = psmt.executeUpdate();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return i;
	}

	// 添加measure
	public int addMeasure(String elementName, String elmentParent) {
		int i = 0;
		String sql = "insert into measure values (null,?,?)";
		PreparedStatement psmt = null;
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			psmt.setString(1, elementName);
			psmt.setString(2, elmentParent);

			i = psmt.executeUpdate();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return i;
	}

	// 添加measure semantic
	public int addMeasureSemantic(String elementName, String elmentSemantic,
			int type) {
		int i = 0;
		String sql = "insert into semantic_measure values (null,?,?,?)";
		PreparedStatement psmt = null;
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			psmt.setString(1, elementName);
			psmt.setString(2, elmentSemantic);
			psmt.setInt(3, type);

			i = psmt.executeUpdate();
			psmt.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
		return i;
	}

	// 删除数据库中所有的表
	public void deleteAllTable() {
		String sql = "show tables";
		PreparedStatement psmt = null;
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			ResultSet rs = psmt.executeQuery();
			while (rs.next()) {
				String tableName = rs.getString(1);
				System.out.println(tableName);
				this.deleteTable(tableName);
			}
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}

	}

	// 删除数据库中的表
	public boolean deleteTable(String tableName) {
		boolean delResult = false;
		String sql = "truncate table " + tableName;
		PreparedStatement psmt = null;
		try {
			psmt = dbc.getConn().prepareStatement(sql);
			psmt.executeUpdate();
			delResult = true;
		} catch (SQLException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
		if (delResult) {
			return true;
		} else {
			return false;
		}

	}

	public static void main(String[] args) {
		System.out.println("-----------------------------");
		// new PolicyDAO().clearDataBaseData();
		// System.out.println(new PolicyDAO().getUserAmount());
		/*
		 * List<String> slist = new PolicyDAO().getInfoList("bob"); for(String
		 * str:slist){ System.out.println(str); }
		 */
		/*
		 * List<CndPolicy> plist = new PolicyDAO().getPolicy(); for(CndPolicy
		 * element:plist){
		 * System.out.println(element.getPolicyId()+" "+element.getContext
		 * ()+" "+element.getRole()+" "+element.getView()
		 * +" "+element.getActivity()+" "+element.getMeasure()); } List<String>
		 * slist = new PolicyDAO().getRoleElementParentList("A"); for(String
		 * str:slist){ System.out.println(str); } List<Map.Entry<String,
		 * String>> mapList = null; HashMap<String,String> hashMap = new
		 * PolicyDAO().getContextMap(); mapList = new
		 * ArrayList<Map.Entry<String, String>>(hashMap.entrySet());
		 * for(Map.Entry<String,String> element:mapList){
		 * System.out.println(element.getKey()+" "+element.getValue()); }
		 * 
		 * CndPolicy policy = new PolicyDAO().getOnePolicy(1);
		 * System.out.println(policy.getPolicyId()+" "+
		 * policy.getContext()+" "+policy.getRole());
		 * System.out.println("the total number of policy :" + new
		 * PolicyDAO().getPolicyAmount()); System.out.println(new
		 * PolicyDAO().addPolicy(policy)); System.out.println(new
		 * PolicyDAO().getMeasureElementDenyList("permit"));
		 * System.out.println(new PolicyDAO().addRoleSemantic("P", "666"));
		 * System.out.println(new PolicyDAO().getSemanticContext());
		 */
		/*
		 * List<Result> rlist = new PolicyDAO().getResultList(); for(Result
		 * element:rlist){
		 * System.out.println(element.getPolicyAmount()+"  "+element
		 * .getConflictAmount
		 * ()+"  "+element.getUnrelation()+"  "+element.getSyntaxconflict
		 * ()+"  "+element.getInclusivematch()+"  "+element.getPartialmatch());
		 * }
		 */

	}
}

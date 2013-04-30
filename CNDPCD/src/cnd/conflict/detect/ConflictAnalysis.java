package cnd.conflict.detect;

import java.util.ArrayList;
import java.util.List;

import cnd.conflict.dao.PolicyDAO;
import cnd.conflict.entity.CndPolicy;
import cnd.conflict.entity.Result;
import cnd.conflict.frame.Frame;

/**
 * @author YC
 * 
 */
public class ConflictAnalysis {

	/**
	 * @param args
	 */
	private static final int UNRELATION = 0;// 无冲突
	private static final int SYNTAXCONFLICT = 1;// 语法冲突
	private static final int INCLUSIVEMATCH = 2;// 包含冲突
	private static final int PARTIALMATCH = 3;// 相关冲突
	private ElementRelation elementRelation = new ElementRelation();
	private static ConflictAnalysis conflictAnalysis = null;

	private String fileActPos = null;
	// 表示两条策略的包含、包含于关系，0为包含（前者包含后者）， 1为包含于, 2为相等
	private int fileRoleInc = 0;
	private int fileActInc = 0;

	private PolicyDAO policyDAO = new PolicyDAO();

	// private Frame frame = new Frame();
	public int judge_Relation_Policy(CndPolicy policy1, CndPolicy policy2) {
		// TODO 判断策略关系
		List<Integer> iList = new ArrayList<Integer>();
		if (policy1.getContext().equals(policy2.getContext())
				&& policy1.getRole().equals(policy2.getRole())
				&& policy1.getView().equals(policy2.getView())
				&& policy1.getActivity().equals(policy2.getActivity())) {
			return SYNTAXCONFLICT;
		}
		iList.add(elementRelation.judge_Relation_Context(policy1.getContext(),
				policy2.getContext()));
		iList.add(elementRelation.judge_Relation_Role(policy1.getRole(),
				policy2.getRole()));
		iList.add(elementRelation.judge_Relation_View(policy1.getView(),
				policy2.getView()));
		iList.add(elementRelation.judge_Relation_Activity(
				policy1.getActivity(), policy2.getActivity()));
		if (iList.contains(0)) {
			return UNRELATION;
		} else {
			for (Integer element : iList) {
				if (iList.contains(1)) {
					return PARTIALMATCH;
				}
			}
			if (iList.contains(3) && iList.contains(4)) {
				return PARTIALMATCH;
			} else {
				return INCLUSIVEMATCH;
			}
		}

	}

	// 文件访问控制判断关系
	public int judge_Relation_Policy(CndPolicy policy1, CndPolicy policy2,
			String dataBase) {

		// TODO 判断策略关系
		List<Integer> iList = new ArrayList<Integer>();
		/*
		 * if (policy1.getOrganization().equals(policy2.getOrganization()) &&
		 * policy1.getContext().equals(policy2.getContext()) &&
		 * policy1.getRole().equals(policy2.getRole()) &&
		 * policy1.getView().equals(policy2.getView()) &&
		 * policy1.getActivity().equals(policy2.getActivity())) { return
		 * SYNTAXCONFLICT; }
		 */
		// 依次判断组织 上下文 文件夹 不一样则肯定无关 即只存在包含和无关两种情况 注意唯一标识
		// 组织
		iList.add(elementRelation.judge_Relation_Organization(policy1
				.getOrganization(), policy2.getOrganization(), dataBase));
		// 上下文
		iList.add(elementRelation.judge_Relation_Context(policy1
				.getOrganization()
				+ "\\" + policy1.getContext(), policy2.getOrganization() + "\\"
				+ policy2.getContext(), dataBase));
		// 文件夹
		iList.add(elementRelation.judge_Relation_View(policy1.getOrganization()
				+ "\\" + policy1.getContext() + "\\" + policy1.getView(),
				policy2.getOrganization() + "\\" + policy2.getContext() + "\\"
						+ policy2.getView(), dataBase));
		// 角色
		iList.add(elementRelation.judge_Relation_Role(policy1.getRole(),
				policy2.getRole(), dataBase));
		// 活动
		iList.add(elementRelation.judge_Relation_Activity(
				policy1.getActivity(), policy2.getActivity(), dataBase));

		if (iList.contains(0)) {
			return UNRELATION;
		} else {
			for (Integer element : iList) {
				if (iList.contains(1)) {
					return PARTIALMATCH;
				}
			}
			if (iList.contains(3) && iList.contains(4)) {
				return PARTIALMATCH;
			} else {
				return INCLUSIVEMATCH;
			}
		}
	}

	public String conflict_Analysis(Frame frame, String dataBase) {
		CndPolicy policy1 = null, policy2 = null;
		Result result = new Result();
		int numberOfPolicy = policyDAO.getPolicyAmount();
		StringBuffer strTotal = new StringBuffer();
		int policyAmount = numberOfPolicy;
		int conflictAmount = 0;
		int unralation = 0;
		int syntaxConflict = 0;
		int inclusiveConflict = 0;
		int partialConflict = 0;

		for (int i = 1; i < numberOfPolicy; i++) {
			for (int j = i + 1; j <= numberOfPolicy; j++) {
				policy1 = policyDAO.getOnePolicy(i);
				policy2 = policyDAO.getOnePolicy(j);
				String str = null;
				String str_Policy1 = "policy" + policy1.getPolicyId();
				String str_Policy2 = "policy" + policy2.getPolicyId();
				if (!(policy1.getMeasure().equals(policy2.getMeasure()))) {
					if (judge_Relation_Policy(policy1, policy2, dataBase) == 0) {
						str = "无冲突";
						unralation++;
					} else {
						/*
						 * if (judge_Relation_Policy(policy1, policy2, dataBase)
						 * == 1) { str = "语法冲突"; StringBuffer strSyntax = new
						 * StringBuffer(); strSyntax.append("\n");
						 * strSyntax.append("冲突对象："); strSyntax.append("\n");
						 * strSyntax.append(str_Policy1 + " " + str_Policy2);
						 * strSyntax.append("\n"); strSyntax.append("冲突位置：");
						 * strSyntax.append("\n");
						 * strSyntax.append("all policy elements");
						 * strSyntax.append("\n"); strSyntax.append("冲突原因：");
						 * strSyntax.append("\n");
						 * strSyntax.append("systax conflict"); str = str +
						 * strSyntax.toString(); syntaxConflict++; }
						 */
						if (judge_Relation_Policy(policy1, policy2, dataBase) == 2) {
							str = "包含冲突";
							StringBuffer strSyntax = new StringBuffer();
							strSyntax.append("\n");
							strSyntax.append("冲突对象：");
							strSyntax.append("\n");
							strSyntax.append(str_Policy1 + " " + str_Policy2);
							strSyntax.append("\n");
							strSyntax.append("冲突位置：");
							strSyntax.append("\n");
							strSyntax.append(str_Policy1 + "_"
									+ policy1.getRole() + "与" + str_Policy2
									+ "_" + policy2.getRole() + "在对文件夹"
									+ policy1.getOrganization() + "\\"
									+ policy1.getContext() + "\\"
									+ policy1.getView() + "的" + this.fileActPos
									+ "活动中发生冲突");
							strSyntax.append("\n");
							strSyntax.append("冲突原因：");
							strSyntax.append("\n");
							if (0 == this.getFileRoleInc()) {
								strSyntax.append(str_Policy2 + "的角色"
										+ policy2.getRole() + "继承了"
										+ str_Policy1 + "的角色"
										+ policy1.getRole());
							} else if (1 == this.getFileRoleInc()) {
								strSyntax.append(str_Policy1 + "的角色"
										+ policy1.getRole() + "继承了"
										+ str_Policy2 + "的角色"
										+ policy2.getRole());
							} else if (3 == this.getFileRoleInc()) {
								
							} else {
								strSyntax.append(str_Policy1 + "的角色"
										+ policy1.getRole() + "与" + str_Policy2
										+ "的角色" + policy2.getRole() + "相同");
							}
							strSyntax.append("\n");
							strSyntax.append(str_Policy1 + "的活动"
									+ policy1.getActivity() + "与" + str_Policy2
									+ "的活动" + policy2.getActivity() + "存在包含关系"
									+ "(" + this.fileActPos + ")");
							strSyntax.append("\n");
							if (0 == this.getFileActInc()) {
								strSyntax.append(str_Policy1 + "的活动"
										+ policy1.getActivity() + "包含"
										+ str_Policy2 + "的活动");
							} else if (1 == this.getFileActInc()) {
								strSyntax.append(str_Policy1 + "的活动"
										+ policy1.getActivity() + "包含于"
										+ str_Policy2 + "的活动");
							} else if (3 == this.getFileActInc()) {

							} else {
								strSyntax.append(str_Policy1 + "的活动"
										+ policy1.getActivity() + "与"
										+ str_Policy2 + "的活动相同");
							}

							strSyntax.append("\n");
							str = str + strSyntax.toString();

							inclusiveConflict++;
						}
						if (judge_Relation_Policy(policy1, policy2, dataBase) == 3) {
							str = "相关冲突";
							partialConflict++;
							StringBuffer strTemp = new StringBuffer();
							strTemp.append("\n");
							strTemp.append("冲突对象：");
							strTemp.append("\n");
							strTemp.append(str_Policy1 + " " + str_Policy2);
							strTemp.append("\n");
							strTemp.append("冲突位置:");
							strTemp.append("\n");
							strTemp.append(str_Policy1 + "_"
									+ policy1.getRole() + "与" + str_Policy2
									+ "_" + policy2.getRole() + "在对文件夹"
									+ policy1.getOrganization() + "\\"
									+ policy1.getContext() + "\\"
									+ policy1.getView() + "的" + this.fileActPos
									+ "活动中发生冲突");
							strTemp.append("\n");
							strTemp.append("冲突原因：");
							strTemp.append("\n");
							if (0 == this.getFileRoleInc()) {
								strTemp.append(str_Policy2 + "的角色"
										+ policy2.getRole() + "继承了"
										+ str_Policy1 + "的角色"
										+ policy1.getRole());
							} else if (1 == this.getFileRoleInc()) {
								strTemp.append(str_Policy1 + "的角色"
										+ policy1.getRole() + "继承了"
										+ str_Policy2 + "的角色"
										+ policy2.getRole());
							} else if (3 == this.getFileRoleInc()) {
								strTemp.append(str_Policy1 + "的组"
										+ policy1.getRole() + "与" + str_Policy2
										+ "的组" + policy2.getRole() + "存在相同的用户");
							} else {
								strTemp.append(str_Policy1 + "的角色"
										+ policy1.getRole() + "与" + str_Policy2
										+ "的角色" + policy2.getRole() + "相同");
							}
							strTemp.append("\n");
							strTemp.append(str_Policy1 + "的活动"
									+ policy1.getActivity() + "与" + str_Policy2
									+ "的活动" + policy2.getActivity() + "存在交集"
									+ "(" + this.fileActPos + ")");
							strTemp.append("\n");
							// strTemp.append("\n");

							str = str + strTemp.toString();

						}
						StringBuffer strBuffer = new StringBuffer();
						strBuffer.append(str_Policy1);
						strBuffer.append(" and ");
						strBuffer.append(str_Policy2);
						strBuffer.append(" exsit ");
						strBuffer.append(str);
						// frame.getJTextArea().setText(strBuffer.toString());
						System.out.println(strBuffer.toString());

						strTotal.append(strBuffer.toString());
						strTotal.append("\n");
					}
				}
			}
		}
		conflictAmount = syntaxConflict + inclusiveConflict + partialConflict;
		result.setPolicyAmount(policyAmount);
		result.setConflictAmount(conflictAmount);
		result.setUnrelation(unralation);
		// result.setSyntaxconflict(syntaxConflict);
		result.setInclusivematch(inclusiveConflict);
		result.setPartialmatch(partialConflict);
		// 如果策略数量相同则不添加新数据
		if (!policyDAO.getPolicyAmountList().contains(policyAmount)) {
			policyDAO.addResult(result);
		}

		frame.getMessageArea().setText(strTotal.toString());

		return strTotal.toString();
	}

	public String conflict_Analysis(Frame frame) {
		// TODO冲突分析,结果写入冲突报告
		CndPolicy policy1 = null, policy2 = null;
		Result result = new Result();
		int numberOfPolicy = policyDAO.getPolicyAmount();
		StringBuffer strTotal = new StringBuffer();
		int policyAmount = numberOfPolicy;
		int conflictAmount = 0;
		int unralation = 0;
		int syntaxConflict = 0;
		int inclusiveConflict = 0;
		int partialConflict = 0;

		for (int i = 1; i < numberOfPolicy; i++) {
			for (int j = i + 1; j <= numberOfPolicy; j++) {
				policy1 = policyDAO.getOnePolicy(i);
				policy2 = policyDAO.getOnePolicy(j);
				String str = null;
				String str_Policy1 = "policy" + policy1.getPolicyId();
				String str_Policy2 = "policy" + policy2.getPolicyId();
				if (policyDAO.getMeasureElementDenyList(policy1.getMeasure())
						.contains(policy2.getMeasure())) {
					if (judge_Relation_Policy(policy1, policy2) == 0) {
						str = "无冲突";
						unralation++;
					}
					if (judge_Relation_Policy(policy1, policy2) == 1) {
						str = "语法冲突";
						StringBuffer strSyntax = new StringBuffer();
						strSyntax.append("\n");
						strSyntax.append("冲突对象：");
						strSyntax.append("\n");
						strSyntax.append(str_Policy1 + " " + str_Policy2);
						strSyntax.append("\n");
						strSyntax.append("冲突位置：");
						strSyntax.append("\n");
						strSyntax.append("all policy elements");
						strSyntax.append("\n");
						strSyntax.append("冲突原因：");
						strSyntax.append("\n");
						strSyntax.append("systax conflict");
						str = str + strSyntax.toString();
						syntaxConflict++;
					}
					if (judge_Relation_Policy(policy1, policy2) == 2) {
						str = "包含冲突";
						StringBuffer strSyntax = new StringBuffer();
						strSyntax.append("\n");
						strSyntax.append("冲突对象：");
						strSyntax.append("\n");
						strSyntax.append(str_Policy1 + " " + str_Policy2);
						strSyntax.append("\n");
						strSyntax.append("冲突位置：");
						strSyntax.append("\n");
						strSyntax.append("all policy elements");
						strSyntax.append("\n");
						strSyntax.append("冲突原因：");
						strSyntax.append("\n");
						strSyntax.append("inclusive conflict");
						str = str + strSyntax.toString();

						inclusiveConflict++;
					}
					if (judge_Relation_Policy(policy1, policy2) == 3) {
						str = "相关冲突";
						partialConflict++;
						StringBuffer strTemp = new StringBuffer();
						strTemp.append("\n");
						strTemp.append("冲突对象：");
						strTemp.append("\n");
						strTemp.append(str_Policy1 + " " + str_Policy2);
						strTemp.append("\n");
						strTemp.append("冲突位置及原因:");
						if (elementRelation.judge_Relation_Context(policy1
								.getContext(), policy2.getContext()) == 1) {
							strTemp.append("Context 存在交集");
							strTemp.append(",");
							// strTemp.append("\n");
						}
						if (elementRelation.judge_Relation_Role(policy1
								.getRole(), policy2.getRole()) == 1) {
							strTemp.append("Role 存在交集");
							strTemp.append(",");
							// strTemp.append("\n");
						}
						if (elementRelation.judge_Relation_View(policy1
								.getView(), policy2.getView()) == 1) {
							strTemp.append("View 存在交集");
							strTemp.append(",");
							// strTemp.append("\n");
						}
						if (elementRelation.judge_Relation_Activity(policy1
								.getActivity(), policy2.getActivity()) == 1) {
							strTemp.append("Activity 存在交集");
							strTemp.append(",");
							// strTemp.append("\n");
						}

						str = str + strTemp.toString();

					}

				} else {
					str = "无冲突";
					unralation++;
				}
				StringBuffer strBuffer = new StringBuffer();
				strBuffer.append(str_Policy1);
				strBuffer.append(" and ");
				strBuffer.append(str_Policy2);
				strBuffer.append(" exsit ");
				strBuffer.append(str);
				// frame.getJTextArea().setText(strBuffer.toString());
				System.out.println(strBuffer.toString());

				strTotal.append(strBuffer.toString());
				strTotal.append("\n");

			}
		}
		conflictAmount = syntaxConflict + inclusiveConflict + partialConflict;
		result.setPolicyAmount(policyAmount);
		result.setConflictAmount(conflictAmount);
		result.setUnrelation(unralation);
		result.setSyntaxconflict(syntaxConflict);
		result.setInclusivematch(inclusiveConflict);
		result.setPartialmatch(partialConflict);
		// 如果策略数量相同则不添加新数据
		if (!policyDAO.getPolicyAmountList().contains(policyAmount)) {
			policyDAO.addResult(result);
		}

		frame.getMessageArea().setText(strTotal.toString());
		return strTotal.toString();

	}
	
	public String conflict_Analysis_English(Frame frame, String dataBase) {
		CndPolicy policy1 = null, policy2 = null;
		Result result = new Result();
		int numberOfPolicy = policyDAO.getPolicyAmount();
		StringBuffer strTotal = new StringBuffer();
		int policyAmount = numberOfPolicy;
		int conflictAmount = 0;
		int unralation = 0;
		int syntaxConflict = 0;
		int inclusiveConflict = 0;
		int partialConflict = 0;

		for (int i = 1; i < numberOfPolicy; i++) {
			for (int j = i + 1; j <= numberOfPolicy; j++) {
				policy1 = policyDAO.getOnePolicy(i);
				policy2 = policyDAO.getOnePolicy(j);
				String str = null;
				String str_Policy1 = "policy" + policy1.getPolicyId();
				String str_Policy2 = "policy" + policy2.getPolicyId();
				if (!(policy1.getMeasure().equals(policy2.getMeasure()))) {
					if (judge_Relation_Policy(policy1, policy2, dataBase) == 0) {
						str = "No conflict";
						unralation++;
					} else {
						/*
						 * if (judge_Relation_Policy(policy1, policy2, dataBase)
						 * == 1) { str = "语法冲突"; StringBuffer strSyntax = new
						 * StringBuffer(); strSyntax.append("\n");
						 * strSyntax.append("冲突对象："); strSyntax.append("\n");
						 * strSyntax.append(str_Policy1 + " " + str_Policy2);
						 * strSyntax.append("\n"); strSyntax.append("冲突位置：");
						 * strSyntax.append("\n");
						 * strSyntax.append("all policy elements");
						 * strSyntax.append("\n"); strSyntax.append("冲突原因：");
						 * strSyntax.append("\n");
						 * strSyntax.append("systax conflict"); str = str +
						 * strSyntax.toString(); syntaxConflict++; }
						 */
						if (judge_Relation_Policy(policy1, policy2, dataBase) == 2) {
							str = "Inclusion Conflict";
							StringBuffer strSyntax = new StringBuffer();
							strSyntax.append("\n");
							strSyntax.append("Conflict Policy：");
							strSyntax.append("\n");
							strSyntax.append(str_Policy1 + " " + str_Policy2);
							strSyntax.append("\n");
							strSyntax.append("Conflict Position：");
							strSyntax.append("\n");
							strSyntax.append(str_Policy1 + "_"
									+ policy1.getRole() + " have conflict with " + str_Policy2
									+ "_" + policy2.getRole() + " to folder "
									+ policy1.getOrganization() + "\\"
									+ policy1.getContext() + "\\"
									+ policy1.getView() + " with action of " + this.fileActPos
									+ "");
							strSyntax.append("\n");
							strSyntax.append("Conflict Reason：");
							strSyntax.append("\n");
							if (0 == this.getFileRoleInc()) {
								strSyntax.append(str_Policy2 + "'s role "
										+ policy2.getRole() + " inherits "
										+ str_Policy1 + "'s role "
										+ policy1.getRole());
							} else if (1 == this.getFileRoleInc()) {
								strSyntax.append(str_Policy1 + "'s role "
										+ policy1.getRole() + " inherits "
										+ str_Policy2 + "'s role"
										+ policy2.getRole());
							} else if (3 == this.getFileRoleInc()) {
								
							} else {
								strSyntax.append(str_Policy1 + "'s role "
										+ policy1.getRole() + " is the same with " + str_Policy2
										+ "'s role " + policy2.getRole() + "");
							}
							strSyntax.append("\n");
							strSyntax.append(str_Policy1 + "'s activity "
									+ policy1.getActivity() + " and " + str_Policy2
									+ " 's activity " + policy2.getActivity() + " have contain relation "
									+ "(" + this.fileActPos + ")");
							strSyntax.append("\n");
							if (0 == this.getFileActInc()) {
								strSyntax.append(str_Policy1 + "'s activity "
										+ policy1.getActivity() + " contains "
										+ str_Policy2 + "'s activity");
							} else if (1 == this.getFileActInc()) {
								strSyntax.append(str_Policy1 + "'s activity "
										+ policy1.getActivity() + "is contained within "
										+ str_Policy2 + "'s activity");
							} else if (3 == this.getFileActInc()) {

							} else {
								strSyntax.append(str_Policy1 + "'s activity "
										+ policy1.getActivity() + " is the same with "
										+ str_Policy2 + "'s activity");
							}

							strSyntax.append("\n");
							str = str + strSyntax.toString();

							inclusiveConflict++;
						}
						if (judge_Relation_Policy(policy1, policy2, dataBase) == 3) {
							str = "Relevant Conflict";
							partialConflict++;
							StringBuffer strTemp = new StringBuffer();
							strTemp.append("\n");
							strTemp.append("Conflict Policy：");
							strTemp.append("\n");
							strTemp.append(str_Policy1 + " " + str_Policy2);
							strTemp.append("\n");
							strTemp.append("Conflict Position:");
							strTemp.append("\n");
							strTemp.append(str_Policy1 + "_"
									+ policy1.getRole() + " have conflict with " + str_Policy2
									+ "_" + policy2.getRole() + " to folder "
									+ policy1.getOrganization() + "\\"
									+ policy1.getContext() + "\\"
									+ policy1.getView() + " with action of " + this.fileActPos
									+ "");
							strTemp.append("\n");
							strTemp.append("Conflict Reason：");
							strTemp.append("\n");
							if (0 == this.getFileRoleInc()) {
								strTemp.append(str_Policy2 + "'s role "
										+ policy2.getRole() + " inherits "
										+ str_Policy1 + "'s role "
										+ policy1.getRole());
							} else if (1 == this.getFileRoleInc()) {
								strTemp.append(str_Policy1 + "'s role "
										+ policy1.getRole() + " inherits "
										+ str_Policy2 + "'s role "
										+ policy2.getRole());
							} else if (3 == this.getFileRoleInc()) {
								strTemp.append(str_Policy1 + "'s group "
										+ policy1.getRole() + " and " + str_Policy2
										+ "'s group " + policy2.getRole() + " have the same user");
							} else {
								strTemp.append(str_Policy1 + "'s role "
										+ policy1.getRole() + " is the same with " + str_Policy2
										+ "'s role " + policy2.getRole() + "");
							}
							strTemp.append("\n");
							strTemp.append(str_Policy1 + "'s activity "
									+ policy1.getActivity() + " and " + str_Policy2
									+ "'s activity " + policy2.getActivity() + " have contain relation "
									+ "(" + this.fileActPos + ")");
							strTemp.append("\n");
							// strTemp.append("\n");

							str = str + strTemp.toString();

						}
						StringBuffer strBuffer = new StringBuffer();
						strBuffer.append(str_Policy1);
						strBuffer.append(" and ");
						strBuffer.append(str_Policy2);
						strBuffer.append(" exsit ");
						strBuffer.append(str);
						// frame.getJTextArea().setText(strBuffer.toString());
						System.out.println(strBuffer.toString());

						strTotal.append(strBuffer.toString());
						strTotal.append("\n");
					}
				}
			}
		}
		conflictAmount = syntaxConflict + inclusiveConflict + partialConflict;
		result.setPolicyAmount(policyAmount);
		result.setConflictAmount(conflictAmount);
		result.setUnrelation(unralation);
		// result.setSyntaxconflict(syntaxConflict);
		result.setInclusivematch(inclusiveConflict);
		result.setPartialmatch(partialConflict);
		// 如果策略数量相同则不添加新数据
		if (!policyDAO.getPolicyAmountList().contains(policyAmount)) {
			policyDAO.addResult(result);
		}

		frame.getMessageArea().setText(strTotal.toString());

		return strTotal.toString();
	}

	public String conflict_Analysis_English(Frame frame) {
		// TODO冲突分析,结果写入冲突报告
		CndPolicy policy1 = null, policy2 = null;
		Result result = new Result();
		int numberOfPolicy = policyDAO.getPolicyAmount();
		StringBuffer strTotal = new StringBuffer();
		int policyAmount = numberOfPolicy;
		int conflictAmount = 0;
		int unralation = 0;
		int syntaxConflict = 0;
		int inclusiveConflict = 0;
		int partialConflict = 0;

		for (int i = 1; i < numberOfPolicy; i++) {
			for (int j = i + 1; j <= numberOfPolicy; j++) {
				policy1 = policyDAO.getOnePolicy(i);
				policy2 = policyDAO.getOnePolicy(j);
				String str = null;
				String str_Policy1 = "policy" + policy1.getPolicyId();
				String str_Policy2 = "policy" + policy2.getPolicyId();
				if (policyDAO.getMeasureElementDenyList(policy1.getMeasure())
						.contains(policy2.getMeasure())) {
					if (judge_Relation_Policy(policy1, policy2) == 0) {
						str = "No conflict";
						unralation++;
					}
					if (judge_Relation_Policy(policy1, policy2) == 1) {
						str = "Grammar Conflict";
						StringBuffer strSyntax = new StringBuffer();
						strSyntax.append("\n");
						strSyntax.append("Conflict policy：");
						strSyntax.append("\n");
						strSyntax.append(str_Policy1 + " " + str_Policy2);
						strSyntax.append("\n");
						strSyntax.append("Conflict Position：");
						strSyntax.append("\n");
						strSyntax.append("all policy elements");
						strSyntax.append("\n");
						strSyntax.append("Conflict Reason：");
						strSyntax.append("\n");
						strSyntax.append("systax conflict");
						str = str + strSyntax.toString();
						syntaxConflict++;
					}
					if (judge_Relation_Policy(policy1, policy2) == 2) {
						str = "Inclusion Conflict";
						StringBuffer strSyntax = new StringBuffer();
						strSyntax.append("\n");
						strSyntax.append("Conflict Policy：");
						strSyntax.append("\n");
						strSyntax.append(str_Policy1 + " " + str_Policy2);
						strSyntax.append("\n");
						strSyntax.append("Conflict Position：");
						strSyntax.append("\n");
						strSyntax.append("all policy elements");
						strSyntax.append("\n");
						strSyntax.append("Conflict Reason：");
						strSyntax.append("\n");
						strSyntax.append("inclusive conflict");
						str = str + strSyntax.toString();

						inclusiveConflict++;
					}
					if (judge_Relation_Policy(policy1, policy2) == 3) {
						str = "Revelant Conflict";
						partialConflict++;
						StringBuffer strTemp = new StringBuffer();
						strTemp.append("\n");
						strTemp.append("Conflict Policy：");
						strTemp.append("\n");
						strTemp.append(str_Policy1 + " " + str_Policy2);
						strTemp.append("\n");
						strTemp.append("Conflict Position & Reason:");
						if (elementRelation.judge_Relation_Context(policy1
								.getContext(), policy2.getContext()) == 1) {
							strTemp.append("Context have overlapped");
							strTemp.append(",");
							// strTemp.append("\n");
						}
						if (elementRelation.judge_Relation_Role(policy1
								.getRole(), policy2.getRole()) == 1) {
							strTemp.append("Role have overlapped");
							strTemp.append(",");
							// strTemp.append("\n");
						}
						if (elementRelation.judge_Relation_View(policy1
								.getView(), policy2.getView()) == 1) {
							strTemp.append("View have overlapped");
							strTemp.append(",");
							// strTemp.append("\n");
						}
						if (elementRelation.judge_Relation_Activity(policy1
								.getActivity(), policy2.getActivity()) == 1) {
							strTemp.append("Activity have overlapped");
							strTemp.append(",");
							// strTemp.append("\n");
						}

						str = str + strTemp.toString();

					}

				} else {
					str = "No conflict";
					unralation++;
				}
				StringBuffer strBuffer = new StringBuffer();
				strBuffer.append(str_Policy1);
				strBuffer.append(" and ");
				strBuffer.append(str_Policy2);
				strBuffer.append(" exsit ");
				strBuffer.append(str);
				// frame.getJTextArea().setText(strBuffer.toString());
				System.out.println(strBuffer.toString());

				strTotal.append(strBuffer.toString());
				strTotal.append("\n");

			}
		}
		conflictAmount = syntaxConflict + inclusiveConflict + partialConflict;
		result.setPolicyAmount(policyAmount);
		result.setConflictAmount(conflictAmount);
		result.setUnrelation(unralation);
		result.setSyntaxconflict(syntaxConflict);
		result.setInclusivematch(inclusiveConflict);
		result.setPartialmatch(partialConflict);
		// 如果策略数量相同则不添加新数据
		if (!policyDAO.getPolicyAmountList().contains(policyAmount)) {
			policyDAO.addResult(result);
		}

		frame.getMessageArea().setText(strTotal.toString());
		return strTotal.toString();

	}

	/*
	 * private ConflictAnalysis(){
	 * 
	 * }
	 */
	/**
	 * 获取单态实例
	 * 
	 * @return RelationGraph
	 */
	public static ConflictAnalysis getInstance() {
		if (conflictAnalysis == null) {
			conflictAnalysis = new ConflictAnalysis();
		}
		return conflictAnalysis;
	}

	/*
	 * public static void main(String[] args) { // TODO Auto-generated method
	 * stub
	 * 
	 * new ConflictAnalysis().test();
	 * 
	 * }
	 */

	public void setFileActPos(String fileActPos) {
		this.fileActPos = fileActPos;
	}

	public String getFileActPos() {
		return fileActPos;
	}

	public void setFileRoleInc(int fileRoleInc) {
		this.fileRoleInc = fileRoleInc;
	}

	public int getFileRoleInc() {
		return fileRoleInc;
	}

	public void setFileActInc(int fileActInc) {
		this.fileActInc = fileActInc;
	}

	public int getFileActInc() {
		return fileActInc;
	}

}

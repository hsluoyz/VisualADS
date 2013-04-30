package cnd.conflict.entity;

/**
 * 
 * @author YC
 * ·ÀÓù²ßÂÔÊµÌå
 */
public class CndPolicy {

	
	private int policyId;
	private int type;
	private String organization;
	private String context;
	private String role;
	private String view;
	private String activity;
	private String measure;
	
	public String getContext() {
		return context;
	}
	public void setContext(String context) {
		this.context = context;
	}
	public String getRole() {
		return role;
	}
	public void setRole(String role) {
		this.role = role;
	}
	public String getView() {
		return view;
	}
	public void setView(String view) {
		this.view = view;
	}
	public String getActivity() {
		return activity;
	}
	public void setActivity(String activity) {
		this.activity = activity;
	}
	public String getMeasure() {
		return measure;
	}
	public void setMeasure(String measure) {
		this.measure = measure;
	}
	public void setPolicyId(int policyId) {
		this.policyId = policyId;
	}
	public int getPolicyId() {
		return policyId;
	}
	public void setType(int type) {
		this.type = type;
	}
	public int getType() {
		return type;
	}
	public void setOrganization(String organization) {
		this.organization = organization;
	}
	public String getOrganization() {
		return organization;
	}
}

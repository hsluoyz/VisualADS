VisualADS
==========
Jan 2012 - Present

BUAA, Key Laboratory of Beijing Network Technology

##Project Description

The project mainly researched on RBAC policy conflict analyzing technique in Microsoft Active Directory surroundings. We have achieved automatic extraction of users, groups, shared resources and access control lists. Conflict detection approaches respectively based on colored petri net and description logic have been proposed, detection reports can be retrieved which consist of conflict locations, types and reasons.

## Responsibility

I am responsible for the system implementation of domain information extraction and conflict detection.

## Introduction

VisualADS stands for VISUAL Active Directory conflict detection System. It's a part of the KLBNT project. VisualADS is a MFC based software running under Windows. Its functionality is to collect the Active Directory data automatically and report the conflicts between permissions from a user or a group to a shared folder. VisualADS facilitates a lot of libraries and tools. It adopts ADSI (Active Directory Service Interface) API to collect the information of domain users, groups, computers and organizations automatically from the DC (Domain Controller). Then it uses the Netshare and WNet APIs to query the details of shared folders for every member computer within the domain. Here we get all the information like which user or group is permited or denied to which shared folder under a read, write or an execute action. We call this kind of permission a "policy". In order to visualize the policies, VisualADS utilizes a graphical library called E-XD++ to display the ograph in the MFC client area. There are so many nodes in the graph and it's not easy to arrange the positions of all nodes. We found that graphviz can parse the dot file and generate a picture in which all graphical elements are arranged very well. So we use graphviz to generate position for each element in our policy, like user, group, computer, shared folder and so on. To detect the conflicts between policies, we have a research on a detecting approach based on DL (Description Logic). Here in VisualADS, we use RacerPro as our DL reasoning engine. We write TBox and SWRL (Semantic Web Rule Language) rules mannually and put forward a algorithm which can transfer policies to ABox automatically. The we use socket to transmit TBox, ABox and SWRL rules into RacerPro engine. And nRQL (new Racer Query Language) is adopted to query the conflict details, like conflict positions, conflict reasons and so on.

## Languages used by VisualADS are: 

C/C++

## Techniques used by VisualADS are: 

MFC for UI 
BCGControlBar for UI 
Graphviz for UI 
E-XD++ for UI 
ADSI API for Active Directory information retrieval 
Network Share API for shared folder information retrieval 
WNet API for shared folder ACL retrieval 
Registry API for shared folder ACL retrieval 
Event Log API for remote log retrieval 
WMI API for system information retrieval 
Winsock API for communications with RacerPro

## Usage: 

1. Copy VisualADS.exe from the "Debug" directory to the "dlls" directory, then execute it. 
2. Fill in the "DC Host or IP" with the Domain Controller hostname or IP address, fill in the "Admin Username" and "Admin Password" with the username and password of the domain administrator of Active Directory, usually named "Administrator". Click "Add Domain" if you want to retrieve multi-domain informatioon simultaneously. Then click "Log on" button and wait until the information be retrieved from the DC. Current states are displayed in the "Console" tab of "Output" window. When finished, the client area will display a graph which visualized the whole domain(s). 
3. Click "Start RacerPro" to start the RacerPro reasoning engine. After the window of RacerPro opens, click "Detect" to detect the policy conflicts between Active Directory policies. The "RacerPro TCP Cache" tab of "Output" window will show the TBox, SWRL rules and ABox communicated with RacerPro. And the "Conflict Detection Result" tab will display the conflict detection report.

## Useful links: 

Download source from GitHub
https://github.com/hsluoyz/VisualADS

BCGControlBar official website
http://www.bcgsoft.com

Graphviz official website 
http://www.graphviz.org

E-XD++ official website 
http://extremediagram.com 

Microsoft Windows 2003 Platform SDK download page 
http://www.microsoft.com/en-us/download/details.aspx?id=12261 

RacerPro official website 
http://www.racer-systems.com

## Pictures:

Please refer to: http://yang-luo.com/visualads.htm

Fig.1. The concept model of Virtualized RBAC.

Fig.2. An example of Domain Conflict in multi-domain systems. 

Fig.3. An example of Migrationn Conflict in multi-domain systems. 

Fig.4. An example of Separation of Duty Conflict in multi-domain systems. 

Fig.5. The ADCQ algorithm for querying ABox. 

Fig.6. The architecture of the virtualized environment. 

Fig.7. The sketch map of the virtualized network. 

Fig.8. The conflict detail diagram provided by VisualADS. 

Fig.9. The main user interface of VisualADS.

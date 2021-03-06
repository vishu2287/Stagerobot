#include <ros/ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int16MultiArray.h>
#include <sensor_msgs/Range.h>
#include <geometry_msgs/PoseWithCovariance.h>
#include <geometry_msgs/TransformStamped.h>
#include <iostream>

using namespace std;

ros::Publisher pubgauche;
ros::Publisher pubdroite;
ros::Subscriber subaruco;

std_msgs::Int16 vit_g;
std_msgs::Int16 vit_d;


void arucoCallBack(const geometry_msgs::TransformStamped &transformMsg){
  	
	if(transformMsg.transform.translation.z > 0.4){
		vit_g.data = 64;
		vit_d.data = 64;

	}	

	else if(transformMsg.transform.translation.z < 0.4 && transformMsg.transform.translation.z > 0.1){
		if(transformMsg.transform.translation.x < 0.02 && transformMsg.transform.translation.x > -0.02){
			vit_g.data = 32;
			vit_d.data = 32;
		}
		else if(transformMsg.transform.translation.x > 0.02){
			vit_g.data = 32;
			vit_d.data = 25;
		}		
		else if(transformMsg.transform.translation.x < -0.02){
			vit_g.data = 25;
			vit_d.data = 32;
		}
	}

	else{
		vit_g.data = 0;
		vit_d.data = 0;
	}


	ROS_INFO("z= %f , x = %f", transformMsg.transform.translation.z, transformMsg.transform.translation.x);
	ROS_INFO("advertise the info gauche %d, droite %d", vit_g.data,vit_d.data);

	ros::Rate loop_rate(10);
	
	while(ros::ok()){
		pubgauche.publish(vit_g);
		pubdroite.publish(vit_d);
		ros::spinOnce();
		loop_rate.sleep();
	}


}



int main(int argc, char** argv){
	ros::init(argc, argv, "cam_moteurs_node");
	ros::NodeHandle nh;
	pubgauche = nh.advertise<std_msgs::Int16>("/moteurgauche",10);
	pubdroite = nh.advertise<std_msgs::Int16>("/moteurdroite",10);
	
	
	subaruco = nh.subscribe("/aruco_single/transform",10,&arucoCallBack);
	ros::spin();

}



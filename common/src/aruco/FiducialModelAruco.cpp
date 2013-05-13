//#include "../../../../cob_object_perception_intern/windows/src/PreCompiledHeaders/StdAfx.h"
#ifdef __LINUX__
	#include "cob_fiducials/FiducialModelAruco.h"
#else
	#include "cob_object_perception/cob_fiducials/common/include/cob_fiducials/aruco/FiducialModelAruco.h"
#endif
#include <opencv/highgui.h>

using namespace ipa_Fiducials;


FiducialModelAruco::FiducialModelAruco()
{
	int pyrDownLevels = 3;
	int speed_level = 0; //0: slow but exact, 1: fast, but inaccurate
	m_marker_size=-1;

	//Configure other parameters
    if (pyrDownLevels>0)
        MDetector.pyrDown(pyrDownLevels);

	MDetector.setCornerRefinementMethod(aruco::MarkerDetector::LINES);
	MDetector.setDesiredSpeed(0);
}

FiducialModelAruco::~FiducialModelAruco()
{
	
}


unsigned long FiducialModelAruco::GetPose(cv::Mat& image, std::vector<t_pose>& vec_pose)
{
	std::vector<aruco::Marker> markers;
	aruco::CameraParameters camera_parameters;
	camera_parameters.setParams(GetCameraMatrix(), GetDistortionCoeffs(), image.size());

	try
	{
		MDetector.detect(image, markers, camera_parameters, m_marker_size);
	} 
	catch(std::exception &ex)
	{
		std::cout << "Exception: " << ex.what() << std::endl;
		return ipa_Utils::RET_FAILED;
	}

	for (int i=0; i<markers.size(); i++)
	{
		t_pose pose;
		
		// Apply transformation
		cv::Mat trans_1x3_CfromO = markers[i].Tvec.clone();
		cv::Mat rot_3x3_CfromO;
		cv::Rodrigues(markers[i].Rvec, rot_3x3_CfromO);

		ApplyExtrinsics(rot_3x3_CfromO, trans_1x3_CfromO);

		rot_3x3_CfromO.copyTo(pose.rot);
		trans_1x3_CfromO.copyTo(pose.trans);

		vec_pose.push_back(pose);
	}

	return ipa_Utils::RET_OK;
}

unsigned long FiducialModelAruco::LoadParameters(std::vector<FiducialArucoParameters> pi_tags)
{
	
	return ipa_Utils::RET_OK;
}

unsigned long FiducialModelAruco::LoadParameters(std::string directory_and_filename)
{
	

	return ipa_Utils::RET_OK;
}



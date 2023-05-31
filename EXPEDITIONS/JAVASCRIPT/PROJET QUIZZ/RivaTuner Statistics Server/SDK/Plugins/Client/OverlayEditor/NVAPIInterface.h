// NVAPIInterface.h: interface for the CNVAPIInterface class.
//
// created by Unwinder
//////////////////////////////////////////////////////////////////////
#pragma once
//////////////////////////////////////////////////////////////////////
#include "nvapi.h"
//////////////////////////////////////////////////////////////////////
#define NV_GPU_POWER_MONITOR_POWER_CHANNELS_MAX	32
//////////////////////////////////////////////////////////////////////
typedef enum _NV_GPU_POWER_MONITOR_POWER_CHANNEL_TYPE
{
	NV_GPU_POWER_MONITOR_POWER_CHANNEL_TYPE_DEFAULT								= 0,
	NV_GPU_POWER_MONITOR_POWER_CHANNEL_TYPE_SUMMATION							= 1,
	NV_GPU_POWER_MONITOR_POWER_CHANNEL_TYPE_ESTIMATION							= 2,
	NV_GPU_POWER_MONITOR_POWER_CHANNEL_TYPE_SLOW								= 3,
	NV_GPU_POWER_MONITOR_POWER_CHANNEL_TYPE_GEMINI_CORRECTION					= 4,
	NV_GPU_POWER_MONITOR_POWER_CHANNEL_TYPE_1X									= 5,
	NV_GPU_POWER_MONITOR_POWER_CHANNEL_TYPE_SENSOR								= 6,
	NV_GPU_POWER_MONITOR_POWER_CHANNEL_TYPE_PSTATE_ESTIMATION_LUT				= 7,
	NV_GPU_POWER_MONITOR_POWER_CHANNEL_TYPE_SENSOR_CLIENT_ALIGNED				= 8,
} NV_GPU_POWER_MONITOR_POWER_CHANNEL_TYPE;
//////////////////////////////////////////////////////////////////////
typedef enum _NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_TYPE
{
	NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_TYPE_WEIGHT					= 0,
	NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_TYPE_BALANCED_PHASE_EST		= 1,
	NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_TYPE_BALANCING_PWM_WEIGHT	= 2,
	NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_TYPE_REGULATOR_LOSS_EST		= 3,
	NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_TYPE_REGULATOR_LOSS_DYN		= 4,
	NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_TYPE_UNKNOWN				= -1,
} NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_TYPE;
//////////////////////////////////////////////////////////////////////
typedef enum _NV_GPU_POWER_CHANNEL_POWER_RAIL
{
	NV_GPU_POWER_CHANNEL_POWER_RAIL_UNKNOWN										= 0,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_OUTPUT_NVVDD								= 1,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_OUTPUT_FBVDD								= 2,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_OUTPUT_FBVDDQ								= 3,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_OUTPUT_FBVDD_Q								= 4,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_OUTPUT_PEXVDD								= 5,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_OUTPUT_A3V3									= 6,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_OUTPUT_3V3NV								= 7,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_OUTPUT_TOTAL_GPU							= 8,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_OUTPUT_FBVDDQ_GPU							= 9,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_OUTPUT_FBVDDQ_MEM							= 10,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_OUTPUT_SRAM									= 11,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_PEX12V1								= 222,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_TOTAL_BOARD2							= 223,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_HIGH_VOLT0							= 224,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_HIGH_VOLT1							= 225,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_NVVDD1								= 226,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_NVVDD2								= 227,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_EXT12V_8PIN2							= 228,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_EXT12V_8PIN3							= 229,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_EXT12V_8PIN4							= 230,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_EXT12V_8PIN5							= 231,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_MISC0									= 232,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_MISC1									= 233,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_MISC2									= 234,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_MISC3									= 235,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_USBC0									= 236,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_USBC1									= 237,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_FAN0									= 238,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_FAN1									= 239,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_SRAM									= 240,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_PWR_SRC_PP							= 241,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_3V3_PP								= 242,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_3V3_MAIN								= 243,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_3V3_AON								= 244,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_TOTAL_BOARD							= 245,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_NVVDD									= 246,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_FBVDD									= 247,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_FBVDDQ								= 248,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_FBVDD_Q								= 249,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_EXT12V_8PIN0							= 250,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_EXT12V_8PIN1							= 251,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_EXT12V_6PIN0							= 252,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_EXT12V_6PIN1							= 253,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_PEX3V3								= 254,
	NV_GPU_POWER_CHANNEL_POWER_RAIL_INPUT_PEX12V								= 255,
} NV_GPU_POWER_CHANNEL_POWER_RAIL;
//////////////////////////////////////////////////////////////////////
typedef struct
{
	NvU32	pwrDeviceMask;
	NvU32	pwrLimitmW;
} NV_GPU_POWER_MONITOR_POWER_CHANNEL_1X_INFO;
//////////////////////////////////////////////////////////////////////
typedef struct
{
	NV_GPU_POWER_MONITOR_POWER_CHANNEL_1X_INFO super;
} NV_GPU_POWER_MONITOR_POWER_CHANNEL_SLOW_INFO;
//////////////////////////////////////////////////////////////////////
typedef struct
{
	NV_GPU_POWER_MONITOR_POWER_CHANNEL_SLOW_INFO super;
} NV_GPU_POWER_MONITOR_POWER_CHANNEL_GEMINI_CORRECTION_INFO;
//////////////////////////////////////////////////////////////////////
typedef struct
{
	NvU8	pwrDevIdx;
	NvU8	pwrDevProvIdx;
} NV_GPU_POWER_MONITOR_POWER_CHANNEL_SENSOR_INFO;
//////////////////////////////////////////////////////////////////////
typedef struct
{
	NvU8	relIdxFirst;
	NvU8	relIdxLast;
} NV_GPU_POWER_MONITOR_POWER_CHANNEL_SUMMATION_INFO;
//////////////////////////////////////////////////////////////////////
typedef struct
{
	NV_GPU_PERF_PSTATE_ID pstateId;
	NvU32	powerOffset;
} NV_GPU_POWER_MONITOR_POWER_CHANNEL_PSTATE_ESTIMATION_LUT_ENTRY_INFO;
//////////////////////////////////////////////////////////////////////
typedef struct
{
	NV_GPU_POWER_MONITOR_POWER_CHANNEL_PSTATE_ESTIMATION_LUT_ENTRY_INFO lutEntry[2];
} NV_GPU_POWER_MONITOR_POWER_CHANNEL_PSTATE_ESTIMATION_LUT_INFO;
//////////////////////////////////////////////////////////////////////
typedef struct
{
	NvS32	weight;
} NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_WEIGHT_INFO;
//////////////////////////////////////////////////////////////////////
typedef struct
{
	NvU8	numTotalPhases;
	NvU8	numStaticPhases;
	NvU8	balancedPhasePolicyRelIdxFirst;
	NvU8	balancedPhasePolicyRelIdxLast;
} NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_BALANCED_PHASE_EST_INFO;
//////////////////////////////////////////////////////////////////////
typedef struct
{
	NvU8	balancingRelIdx;
	NvU8	bPrimary;
} NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_BALANCING_PWM_WEIGHT_INFO;
//////////////////////////////////////////////////////////////////////
typedef struct
{
	NvU8	regulatorType;
	NvS32	coefficient0;
	NvS32	coefficient1;
	NvS32	coefficient2;
	NvS32	coefficient3;
	NvS32	coefficient4;
	NvS32	coefficient5;
} NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_REGULATOR_LOSS_EST_INFO;
//////////////////////////////////////////////////////////////////////
typedef struct
{
	NvU8	thermMonIdx;
	NvU8	voltDomain;
} NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_REGULATOR_LOSS_DYN_INFO;
//////////////////////////////////////////////////////////////////////
typedef struct 
{
	NvU32	pwrDeviceMask;
	NvS32	pwrOffsetmW;
	NvU32	pwrLimitmW;
	NV_GPU_POWER_MONITOR_POWER_CHANNEL_TYPE type;
	NV_GPU_POWER_CHANNEL_POWER_RAIL			pwrRail;
	NvU32	voltFixeduV;
	NvU32	pwrCorrSlope;
	NvU32	currCorrSlope;
	NvS32	currCorrOffsetmA;
	NvU8	rsvd[8];

	union
	{
		NV_GPU_POWER_MONITOR_POWER_CHANNEL_1X_INFO						c1x;
		NV_GPU_POWER_MONITOR_POWER_CHANNEL_GEMINI_CORRECTION_INFO		gemmCorr;
		NV_GPU_POWER_MONITOR_POWER_CHANNEL_SENSOR_INFO					sensor;
		NV_GPU_POWER_MONITOR_POWER_CHANNEL_SLOW_INFO					slow;
		NV_GPU_POWER_MONITOR_POWER_CHANNEL_SUMMATION_INFO				sum;
		NV_GPU_POWER_MONITOR_POWER_CHANNEL_PSTATE_ESTIMATION_LUT_INFO	pstateEstLUT;
		NvU8															rsvd[16];
	} data;
} NV_GPU_POWER_MONITOR_POWER_CHANNEL_INFO_V2;
//////////////////////////////////////////////////////////////////////
typedef struct
{
	NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_TYPE type;
	NvU8 chIdx;

	union 
	{
		NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_WEIGHT_INFO					weight;
		NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_BALANCED_PHASE_EST_INFO		balancedPhaseEst;
		NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_BALANCING_PWM_WEIGHT_INFO	balancingPwmWeight;
		NvU8																		rsvd[16];
	};
} NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_INFO_V2;
//////////////////////////////////////////////////////////////////////
typedef struct
{
	NvU32	version;
	NvBool	bSupported;
	NvU32	samplingPeriodms;
	NvU32	sampleCount;
	NvU32	channelMask;
	NvU32	chRelMask;
	NvU32	totalGpuPowerChannelMask;
	NvU8	totalGpuChannelIdx;
	NvU8	rsvd[8];

	NV_GPU_POWER_MONITOR_POWER_CHANNEL_INFO_V2				channels[NV_GPU_POWER_MONITOR_POWER_CHANNELS_MAX];
	NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_INFO_V2 chRels	[NV_GPU_POWER_MONITOR_POWER_CHANNELS_MAX];
} NV_GPU_POWER_MONITOR_GET_INFO_V2;
//////////////////////////////////////////////////////////////////////
typedef struct
{
	NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_TYPE type;
	NvU8 chIdx;

	union 
	{
		NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_WEIGHT_INFO					weight;
		NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_BALANCED_PHASE_EST_INFO		balancedPhaseEst;
		NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_BALANCING_PWM_WEIGHT_INFO	balancingPwmWeight;
		NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_REGULATOR_LOSS_EST_INFO		regulatorLossEst;
		NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_REGULATOR_LOSS_DYN_INFO		regulatorLossDyn;
		NvU8																		rsvd[32];
	};

} NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_INFO_V3;
//////////////////////////////////////////////////////////////////////
typedef struct 
{
	NvU32	version;
	NvBool	bSupported;
	NvU32	samplingPeriodms;
	NvU32	sampleCount;
	NvU32	channelMask;
	NvU32	chRelMask;
	NvU32	totalGpuPowerChannelMask;
	NvU8	totalGpuChannelIdx;
	NvU8	rsvd[8];

	NV_GPU_POWER_MONITOR_POWER_CHANNEL_INFO_V2				channels[NV_GPU_POWER_MONITOR_POWER_CHANNELS_MAX];
	NV_GPU_POWER_MONITOR_POWER_CHANNEL_RELATIONSHIP_INFO_V3 chRels	[NV_GPU_POWER_MONITOR_POWER_CHANNELS_MAX];
} NV_GPU_POWER_MONITOR_GET_INFO_V3;
//////////////////////////////////////////////////////////////////////
typedef struct 
{
#pragma pack(1)
	NvU32	pwrAvgmW;
	NvU32	pwrMinmW;
	NvU32	pwrMaxmW;
	NvU32	currmA;
	NvU32	voltuV;
	NvU64	energymJ;
    NvU8	rsvd[16];
#pragma pack()
} NV_GPU_POWER_MONITOR_POWER_CHANNEL_STATUS_V2;
//////////////////////////////////////////////////////////////////////
typedef struct 
{
	NvU32	version;
	NvU32	channelMask;
	NvU32	totalGpuPowermW;
    NvU8	rsvd[16];

	NV_GPU_POWER_MONITOR_POWER_CHANNEL_STATUS_V2 channels[NV_GPU_POWER_MONITOR_POWER_CHANNELS_MAX];
} NV_GPU_POWER_MONITOR_GET_STATUS_V2;
//////////////////////////////////////////////////////////////////////
typedef NV_GPU_POWER_MONITOR_GET_INFO_V2	NV_GPU_POWER_MONITOR_GET_INFO;
typedef NV_GPU_POWER_MONITOR_GET_STATUS_V2	NV_GPU_POWER_MONITOR_GET_STATUS;
//////////////////////////////////////////////////////////////////////
#define NVAPIIID_GPU_PowerMonitorGetInfo					0xC12EB19E
#define NVAPIIID_GPU_PowerMonitorGetStatus					0xF40238EF
//////////////////////////////////////////////////////////////////////
typedef LPVOID			(__cdecl * LPNVAPI_QUERYINTERFACE			)(DWORD dwID);
typedef NvAPI_Status	(__cdecl * LPNVAPI_GPU_POWERMONITORGETINFO	)(NvPhysicalGpuHandle hGpu, NV_GPU_POWER_MONITOR_GET_INFO* lpInfo);
typedef NvAPI_Status	(__cdecl * LPNVAPI_GPU_POWERMONITORGETSTATUS)(NvPhysicalGpuHandle hGpu, NV_GPU_POWER_MONITOR_GET_STATUS* lpStatus);
//////////////////////////////////////////////////////////////////////
class CNVAPIInterface
{
public:
	CNVAPIInterface();
	virtual ~CNVAPIInterface();

	LPVOID QueryInterface(DWORD dwIID);

	NvAPI_Status NvAPI_GPU_PowerMonitorGetInfo(NvPhysicalGpuHandle hGpu, NV_GPU_POWER_MONITOR_GET_INFO* lpInfo);
	NvAPI_Status NvAPI_GPU_PowerMonitorGetStatus(NvPhysicalGpuHandle hGpu, NV_GPU_POWER_MONITOR_GET_STATUS* lpStatus);
};
//////////////////////////////////////////////////////////////////////

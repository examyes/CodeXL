//==================================================================================
// Copyright (c) 2013-2016 , Advanced Micro Devices, Inc.  All rights reserved.
//
/// \author AMD Developer Tools Team
/// \file AMDTProfileDbAdapter.h
///
//==================================================================================

#ifndef _AMDTPROFILEDBADAPTER_H_
#define _AMDTPROFILEDBADAPTER_H_

#include <AMDTBaseTools/Include/gtVector.h>
#include <AMDTDbAdapter/inc/AMDTDbAdapter.h>
#include <AMDTCommonHeaders/AMDTCommonProfileDataTypes.h>
#include <AMDTDbAdapter/inc/CPAdapterDataDefs.h>


class AMDTDBADAPTER_API amdtProfileDbAdapter : public AmdtDatabaseAdapter
{
public:
    amdtProfileDbAdapter() {};
    ~amdtProfileDbAdapter() {};
    amdtProfileDbAdapter(const amdtProfileDbAdapter& other) = delete;
    amdtProfileDbAdapter& operator=(const amdtProfileDbAdapter& other) = delete;

    //
    // Insert APIs
    //

    bool InsertAllSessionInfo(const AMDTProfileSessionInfo& sessionInfo,
                              unsigned profilingIntervalMs,
                              const gtVector<AMDTProfileDevice*>& deviceList,
                              const gtVector<AMDTProfileCounterDesc*>& counterList,
                              const gtVector<int>& enabledCounters);

    bool InsertDevice(const AMDTProfileDevice& device);
    bool InsertDevice(const gtVector<AMDTProfileDevice*>& deviceList);
    bool InsertTopology(const CPAdapterTopologyMap& topologyMap);

    bool InsertCounters(const gtVector<AMDTProfileCounterDesc*>& countersList);
    bool InsertCounterEnabled(int counterId, int quantizedTime);
    bool InsertCounterDisabled(int counterId, int quantizedTime);

    bool InsertCounterInfo(AMDTProfileCounterDescVec& events);
    bool InsertSamplingConfigInfo(const AMDTProfileSamplingConfigVec& configVec);
    bool InsertSamplingEvents(AMDTProfileCounterDescVec& events, const AMDTProfileSamplingConfigVec& configVec);

    bool InsertCoreSamplingConfigInfo(const CPACoreSamplingConfigList& coreConfigList);
    bool InsertSamplingInterval(unsigned samplingIntervalMs, unsigned quantizedTime);

    bool InsertProcessInfo(const CPAProcessList& procesList);
    bool InsertProcessThreadInfo(const CPAProcessThreadList& procThreadList);

    bool InsertModuleInfo(const CPAModuleList& moduleList);
    bool InsertModuleInstanceInfo(const CPAModuleInstanceList& moduleInstanceList);

    bool InsertFunctionInfo(const CPAFunctionInfoList& funcList);

    bool InsertSamples(const gtVector<AMDTProfileTimelineSample*>& ppSamples);
    bool InsertSamples(const CPASampeInfoList& sampleList);

    bool InsertCallStackFrames(const CPACallStackFrameInfoList& csFrameInfoList);
    bool InsertCallStackLeafs(const CPACallStackLeafInfoList& csLeafInfoList);

    bool InsertJitInstanceInfo(const CPAJitInstanceInfoList& jitInstanceInfoList);
    bool InsertJitCodeBlobInfo(CPAJitCodeBlobInfoList& jitCodeBlobInfoList);

    //
    // Update APIs
    // mainly used to support the db generated by codexl 1.9
    //

    bool UpdateDeviceTypeId(const gtMap<gtString, int>& deviceInfo);
    bool UpdateCounterCategoryId(const gtMap<gtString, int>& counterInfo);
    bool UpdateCounterAggregationId(const gtMap<gtString, int>& counterInfo);
    bool UpdateCounterUnitId(const gtMap<gtString, int>& counterInfo);

    //
    // Query APIs
    //

    bool GetDeviceType(int deviceId, int& deviceType) const;
    bool GetDeviceTypeByCounterId(int counterId, int& deviceType) const;

    bool GetCounterNames(gtMap<gtString, int>& counterNames);
    bool GetCounterIdByName(const gtString& counterName, int& counterId);

    // TODO: This needs to be simplified
    bool GetCountersDescription(gtMap<int, AMDTProfileCounterDesc>& counterDetails);

    // TODO: This needs to be simplified
    bool GetSessionCounters(int deviceType, int counterCategory, gtVector<int>& counterIds);
    bool GetSessionCounters(int counterCategory, gtVector<int>& counterIds);
    bool GetSessionCounters(const gtVector<int>& deviceTypes, int counterCategory, gtVector<int>& counterIds);

    bool GetSessionSamplingIntervalMs(unsigned& samplingIntervalMs);

    bool GetSessionTimeRange(SamplingTimeRange& samplingTimeRange) const;

    bool GetGlobalMinMaxValuesPerCounters(const gtVector<int> counterIds, SamplingTimeRange& samplingTimeRange, double& minValue, double& maxValue);
    bool GetOverallNubmerOfSamples(const gtVector<int>& counterIds, gtMap<int, int>& numberOfSamplesPerCounter);
    bool GetSampledValuesByRange(const gtVector<int>& counterIds, SamplingTimeRange& samplingTimeRange, gtMap<int, gtVector<SampledValue>>& sampledValuesPerCounter);
    bool GetSampleCountByCounterId(const gtVector<int>& counterIds, gtMap<int, int>& numberOfSamplesPerCounter);
    bool GetSamplesGroupByCounterId(const gtVector<int>& counterIds, gtMap<int, double>& samplesMap);
    bool GetBucketizedSamplesByCounterId(unsigned int bucketWidth,
                                         const gtVector<int>& counterIds,
                                         gtVector<int>& cIds, gtVector<double>& dbBucketBottoms,
                                         gtVector<int>& dbBucketCount);

    bool GetCpuTopology(gtVector<AMDTCpuTopology>& cpuTopology);

    bool GetSampledCountersList(gtVector<AMDTProfileCounterDesc>& counterDesc);

    bool GetSamplingConfiguration(AMDTUInt32 counterId, AMDTProfileSamplingConfig& samplingConfig);

    bool GetProcessInfo(AMDTUInt32 pid, gtVector<AMDTProfileProcessInfo>& processInfoList);
    bool GetModuleInfo(AMDTUInt32 pid, AMDTModuleId mid, gtVector<AMDTProfileModuleInfo>& moduleInfoList);
    bool GetThreadInfo(AMDTUInt32 pid, AMDTThreadId tid, gtVector<AMDTProfileThreadInfo>& threadInfoList);
    bool GetFunctionInfoByModuleId(AMDTModuleId moduleId, AMDTProfileFunctionInfoVec& funcInfoVec);

    bool GetProcessesWithCallstackSamples(gtVector<AMDTProcessId>& cssProcessVec);

    bool GetCounterTotals(AMDTProfileDataType   type,
        AMDTProcessId         procId,
        AMDTThreadId          threadId,
        AMDTModuleId          moduleId,
        AMDTFunctionId        funcId,
        gtVector<AMDTUInt32>  counterIdsList,
        AMDTUInt64            coreMask,
        bool                  separateByCore,
        AMDTSampleValueVec&   sampleValueVec);

    bool GetProfileData(AMDTProfileDataType type,
                        AMDTProcessId processId,   // can be AMDT_CP_ALL_PROCESSES for process summary
                        AMDTModuleId moduleId,     // can be AMDT_CP_ALL_MODULES
                        AMDTThreadId threadId,     // can be AMDT_CP_ALL_THREAD
                        gtVector<AMDTUInt32> counterIdList,
                        AMDTUInt64 coreMask,       // can be AMDT_CP_ALL_CORES
                        bool ignoreSystemModules,
                        bool separateByCore,
                        bool separateByProcess,
                        bool doSort,
                        size_t count,              // AMDT_CP_MAX_COUNT - returns all the entries
                        gtVector<AMDTProfileData>& dataList);

    bool GetJITFunctionInfo(AMDTFunctionId funcId, gtUInt64& loadAddr, gtString& srcFilePath, gtString& jncFilePath);

    bool GetFunctionProfileData(AMDTFunctionId              funcId,
                                gtUInt32                    funcStartOffset,    // used only for unknonwn functions
                                AMDTProcessId               processId,
                                AMDTThreadId                threadId,
                                gtVector<AMDTUInt32>&       counterIdList,
                                AMDTUInt64                  coreMask,
                                bool                        separateByCore,
                                AMDTProfileFunctionData&    functionData);

    // This returns from SampleContext table - IP samples
    bool GetUnknownFunctions(gtVector<AMDTProfileFunctionInfo>& funcList);

    bool GetUnknownCallstackLeafsByProcessId(AMDTProcessId processId, CallstackFrameVec& leafs);

    bool GetCallstackLeafData(AMDTProcessId       processId,
                              AMDTUInt32          counterId,
                              gtUInt32            callStackId,
                              AMDTFunctionId      funcId,
                              gtUInt32            funcOffset,
                              CallstackFrameVec&  leafs);

    bool GetCallstackFrameData(AMDTProcessId       processId,
                               gtUInt32            callstackId,
                               CallstackFrameVec&  frames);

    bool GetCallstackIds(AMDTProcessId        processId,
                         AMDTFunctionId       funcId,
                         gtUInt32             funcOffset,
                         bool                 isLeafEntries,
                         gtVector<gtUInt32>&  csIds);

    bool GetMaxFunctionId(AMDTModuleId moduleId, gtUInt32& maxFuncId);

    bool UpdateCallstackLeaf(const AMDTProfileFunctionInfo& funcInfo);
    bool UpdateCallstackFrame(const AMDTProfileFunctionInfo& funcInfo);
    bool InsertFunctionInfo(const AMDTProfileFunctionInfo& funcInfo);

private:
    void PrepareTimelineSamplesToInsert(AMDTProfileTimelineSample* pSample, gtVector<PPSampleData>& dbSamples);
};

#endif // _AMDTPROFILEDBADAPTER_H_

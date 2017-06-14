#ifndef __MEDIA_OMX_CORE_H__
#define __MEDIA_OMX_CORE_H__

#define OMX_SKIP64BIT
#include <OMX_Component.h>
#ifdef OMX_RPI
#include <OMX_Broadcom.h>
#endif
#include <cstring>

#ifndef OMX_VERSION
  #define OMX_VERSION_MAJOR 1
  #define OMX_VERSION_MINOR 1
  #define OMX_VERSION_REVISION 2
  #define OMX_VERSION_STEP 0
  #define OMX_VERSION ((OMX_VERSION_STEP<<24) | (OMX_VERSION_REVISION<<16) | (OMX_VERSION_MINOR<<8) | OMX_VERSION_MAJOR)
#endif

namespace omx {

template <typename S>
struct header : S {
	header () {
		std::memset(this, 0, sizeof(S));
		S & base_type = static_cast<S&>(*this);
		base_type.nSize = sizeof(S);
		base_type.nVersion.nVersion = OMX_VERSION;
	}
};

struct OMX_PortParam : header<OMX_PORT_PARAM_TYPE> {};
struct OMX_ParamPortDefinition : header<OMX_PARAM_PORTDEFINITIONTYPE> {};
struct OMX_TimeConfigClockState : header<OMX_TIME_CONFIG_CLOCKSTATETYPE> {};
struct OMX_TimeConfigScale : header<OMX_TIME_CONFIG_SCALETYPE> {};
struct OMX_BufferHeader : header<OMX_BUFFERHEADERTYPE> {};
struct OMX_VideoParamPortFormat : header<OMX_VIDEO_PARAM_PORTFORMATTYPE> {};
struct OMX_VideoParamBitrate : header<OMX_VIDEO_PARAM_BITRATETYPE> {};
#ifdef OMX_RPI
struct OMX_ConfigPortBoolean : header<OMX_CONFIG_PORTBOOLEANTYPE> {};
struct OMX_ConfigBrcmPortStats : header<OMX_CONFIG_BRCMPORTSTATSTYPE> {};
#endif

} // namespace omx

#endif // __MEDIA_OMX_CORE_H__

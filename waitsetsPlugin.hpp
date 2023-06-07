

/*
WARNING: THIS FILE IS AUTO-GENERATED. DO NOT MODIFY.

This file was generated from waitsets.idl
using RTI Code Generator (rtiddsgen) version 3.1.0.
The rtiddsgen tool is part of the RTI Connext DDS distribution.
For more information, type 'rtiddsgen -help' at a command shell
or consult the Code Generator User's Manual.
*/

#ifndef waitsetsPlugin_1263072880_h
#define waitsetsPlugin_1263072880_h

#include "waitsets.hpp"

struct RTICdrStream;

#ifndef pres_typePlugin_h
#include "pres/pres_typePlugin.h"
#endif

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, start exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport __declspec(dllexport)
#endif

#define waitsetsPlugin_get_sample PRESTypePluginDefaultEndpointData_getSample

#define waitsetsPlugin_get_buffer PRESTypePluginDefaultEndpointData_getBuffer 
#define waitsetsPlugin_return_buffer PRESTypePluginDefaultEndpointData_returnBuffer

#define waitsetsPlugin_create_sample PRESTypePluginDefaultEndpointData_createSample 
#define waitsetsPlugin_destroy_sample PRESTypePluginDefaultEndpointData_deleteSample 

/* --------------------------------------------------------------------------------------
Support functions:
* -------------------------------------------------------------------------------------- */

NDDSUSERDllExport extern waitsets*
waitsetsPluginSupport_create_data_w_params(
    const struct DDS_TypeAllocationParams_t * alloc_params);

NDDSUSERDllExport extern waitsets*
waitsetsPluginSupport_create_data_ex(RTIBool allocate_pointers);

NDDSUSERDllExport extern waitsets*
waitsetsPluginSupport_create_data(void);

NDDSUSERDllExport extern RTIBool 
waitsetsPluginSupport_copy_data(
    waitsets *out,
    const waitsets *in);

NDDSUSERDllExport extern void 
waitsetsPluginSupport_destroy_data_w_params(
    waitsets *sample,
    const struct DDS_TypeDeallocationParams_t * dealloc_params);

NDDSUSERDllExport extern void 
waitsetsPluginSupport_destroy_data_ex(
    waitsets *sample,RTIBool deallocate_pointers);

NDDSUSERDllExport extern void 
waitsetsPluginSupport_destroy_data(
    waitsets *sample);

NDDSUSERDllExport extern void 
waitsetsPluginSupport_print_data(
    const waitsets *sample,
    const char *desc,
    unsigned int indent);

/* ----------------------------------------------------------------------------
Callback functions:
* ---------------------------------------------------------------------------- */

NDDSUSERDllExport extern PRESTypePluginParticipantData 
waitsetsPlugin_on_participant_attached(
    void *registration_data, 
    const struct PRESTypePluginParticipantInfo *participant_info,
    RTIBool top_level_registration, 
    void *container_plugin_context,
    RTICdrTypeCode *typeCode);

NDDSUSERDllExport extern void 
waitsetsPlugin_on_participant_detached(
    PRESTypePluginParticipantData participant_data);

NDDSUSERDllExport extern PRESTypePluginEndpointData 
waitsetsPlugin_on_endpoint_attached(
    PRESTypePluginParticipantData participant_data,
    const struct PRESTypePluginEndpointInfo *endpoint_info,
    RTIBool top_level_registration, 
    void *container_plugin_context);

NDDSUSERDllExport extern void 
waitsetsPlugin_on_endpoint_detached(
    PRESTypePluginEndpointData endpoint_data);

NDDSUSERDllExport extern void    
waitsetsPlugin_return_sample(
    PRESTypePluginEndpointData endpoint_data,
    waitsets *sample,
    void *handle);    

NDDSUSERDllExport extern RTIBool 
waitsetsPlugin_copy_sample(
    PRESTypePluginEndpointData endpoint_data,
    waitsets *out,
    const waitsets *in);

/* ----------------------------------------------------------------------------
(De)Serialize functions:
* ------------------------------------------------------------------------- */

NDDSUSERDllExport extern RTIBool
waitsetsPlugin_serialize_to_cdr_buffer(
    char * buffer,
    unsigned int * length,
    const waitsets *sample,
    ::dds::core::policy::DataRepresentationId representation
    = ::dds::core::policy::DataRepresentation::xcdr()); 

NDDSUSERDllExport extern RTIBool 
waitsetsPlugin_deserialize(
    PRESTypePluginEndpointData endpoint_data,
    waitsets **sample, 
    RTIBool * drop_sample,
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_sample, 
    void *endpoint_plugin_qos);

NDDSUSERDllExport extern RTIBool
waitsetsPlugin_deserialize_from_cdr_buffer(
    waitsets *sample,
    const char * buffer,
    unsigned int length);    

NDDSUSERDllExport extern unsigned int 
waitsetsPlugin_get_serialized_sample_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

/* --------------------------------------------------------------------------------------
Key Management functions:
* -------------------------------------------------------------------------------------- */
NDDSUSERDllExport extern PRESTypePluginKeyKind 
waitsetsPlugin_get_key_kind(void);

NDDSUSERDllExport extern unsigned int 
waitsetsPlugin_get_serialized_key_max_size(
    PRESTypePluginEndpointData endpoint_data,
    RTIBool include_encapsulation,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern unsigned int 
waitsetsPlugin_get_serialized_key_max_size_for_keyhash(
    PRESTypePluginEndpointData endpoint_data,
    RTIEncapsulationId encapsulation_id,
    unsigned int current_alignment);

NDDSUSERDllExport extern RTIBool 
waitsetsPlugin_deserialize_key(
    PRESTypePluginEndpointData endpoint_data,
    waitsets ** sample,
    RTIBool * drop_sample,
    struct RTICdrStream *stream,
    RTIBool deserialize_encapsulation,
    RTIBool deserialize_key,
    void *endpoint_plugin_qos);

/* Plugin Functions */
NDDSUSERDllExport extern struct PRESTypePlugin*
waitsetsPlugin_new(void);

NDDSUSERDllExport extern void
waitsetsPlugin_delete(struct PRESTypePlugin *);

#if (defined(RTI_WIN32) || defined (RTI_WINCE) || defined(RTI_INTIME)) && defined(NDDS_USER_DLL_EXPORT)
/* If the code is building on Windows, stop exporting symbols.
*/
#undef NDDSUSERDllExport
#define NDDSUSERDllExport
#endif

#endif /* waitsetsPlugin_1263072880_h */


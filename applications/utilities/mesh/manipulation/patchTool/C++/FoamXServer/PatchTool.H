/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <CORBA.h>
#include <mico/throw.h>

#ifndef __PATCHTOOL_H__
#define __PATCHTOOL_H__






namespace FoamXServer
{
namespace PatchToolServer
{

class IPatchToolServer;
typedef IPatchToolServer *IPatchToolServer_ptr;
typedef IPatchToolServer_ptr IPatchToolServerRef;
typedef ObjVar< IPatchToolServer > IPatchToolServer_var;
typedef ObjOut< IPatchToolServer > IPatchToolServer_out;

}
}






namespace FoamXServer
{

enum FoamXType {
  Type_Undefined = 0,
  Type_Boolean,
  Type_Label,
  Type_Scalar,
  Type_Char,
  Type_Word,
  Type_String,
  Type_RootDir,
  Type_RootAndCase,
  Type_CaseName,
  Type_HostName,
  Type_File,
  Type_Directory,
  Type_Time,
  Type_DimensionSet,
  Type_VectorSpace,
  Type_List,
  Type_Dictionary,
  Type_Selection,
  Type_Compound,
  Type_Field
};

typedef FoamXType& FoamXType_out;

extern CORBA::TypeCodeConst _tc_FoamXType;

enum ErrorCode {
  S_OK = 0,
  E_FAIL,
  E_FOAM,
  E_INVALID_ARG,
  E_INVALID_PTR,
  E_INVALID_REF,
  E_INDEX_OUT_OF_BOUNDS,
  E_UNKNOWN_NAME,
  E_UNEXPECTED
};

typedef ErrorCode& ErrorCode_out;

extern CORBA::TypeCodeConst _tc_ErrorCode;

enum JobStatus {
  JOB_UNDEFINED = 0,
  JOB_LAUNCHING,
  JOB_RUNNING,
  JOB_STOPPING,
  JOB_SUSPENDED,
  JOB_FINISHED,
  JOB_ABORTED
};

typedef JobStatus& JobStatus_out;

extern CORBA::TypeCodeConst _tc_JobStatus;

enum MessageType {
  M_DIAGNOSTIC = 0,
  M_WARNING,
  M_ERROR
};

typedef MessageType& MessageType_out;

extern CORBA::TypeCodeConst _tc_MessageType;

typedef StringSequenceTmpl<CORBA::String_var> StringList;
typedef TSeqVar< StringSequenceTmpl<CORBA::String_var> > StringList_var;
typedef TSeqOut< StringSequenceTmpl<CORBA::String_var> > StringList_out;

extern CORBA::TypeCodeConst _tc_StringList;

typedef SequenceTmpl< CORBA::Double,MICO_TID_DEF> DoubleList;
typedef TSeqVar< SequenceTmpl< CORBA::Double,MICO_TID_DEF> > DoubleList_var;
typedef TSeqOut< SequenceTmpl< CORBA::Double,MICO_TID_DEF> > DoubleList_out;

extern CORBA::TypeCodeConst _tc_DoubleList;

typedef SequenceTmpl< CORBA::Float,MICO_TID_DEF> FloatList;
typedef TSeqVar< SequenceTmpl< CORBA::Float,MICO_TID_DEF> > FloatList_var;
typedef TSeqOut< SequenceTmpl< CORBA::Float,MICO_TID_DEF> > FloatList_out;

extern CORBA::TypeCodeConst _tc_FloatList;

typedef SequenceTmpl< CORBA::Boolean,MICO_TID_BOOL> BoolList;
typedef TSeqVar< SequenceTmpl< CORBA::Boolean,MICO_TID_BOOL> > BoolList_var;
typedef TSeqOut< SequenceTmpl< CORBA::Boolean,MICO_TID_BOOL> > BoolList_out;

extern CORBA::TypeCodeConst _tc_BoolList;

typedef SequenceTmpl< CORBA::Long,MICO_TID_DEF> LongList;
typedef TSeqVar< SequenceTmpl< CORBA::Long,MICO_TID_DEF> > LongList_var;
typedef TSeqOut< SequenceTmpl< CORBA::Long,MICO_TID_DEF> > LongList_out;

extern CORBA::TypeCodeConst _tc_LongList;

typedef SequenceTmpl< LongList,MICO_TID_DEF> LongLongList;
typedef TSeqVar< SequenceTmpl< LongList,MICO_TID_DEF> > LongLongList_var;
typedef TSeqOut< SequenceTmpl< LongList,MICO_TID_DEF> > LongLongList_out;

extern CORBA::TypeCodeConst _tc_LongLongList;

typedef CORBA::Float Point3[ 3 ];
typedef CORBA::Float Point3_slice;
typedef ArrayFixVar< CORBA::Float,Point3_slice,Point3,3 > Point3_var;
typedef Point3 Point3_out;
enum _dummy_Point3 { _dummy_Point3_0 };
typedef ArrayFixForAny < CORBA::Float,Point3_slice,Point3,3,_dummy_Point3> Point3_forany;
#undef MICO_ARRAY_ARG
#define MICO_ARRAY_ARG CORBA::Float
DECLARE_ARRAY_ALLOC(Point3,MICO_ARRAY_ARG,Point3_slice,3)
DECLARE_ARRAY_DUP(Point3,MICO_ARRAY_ARG,Point3_slice,3)
DECLARE_ARRAY_FREE(Point3,MICO_ARRAY_ARG,Point3_slice,3)
DECLARE_ARRAY_COPY(Point3,MICO_ARRAY_ARG,Point3_slice,3)
extern CORBA::TypeCodeConst _tc_Point3;

struct FoamXAny;
typedef TVarVar< FoamXAny > FoamXAny_var;
typedef TVarOut< FoamXAny > FoamXAny_out;


struct FoamXAny {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef FoamXAny_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  FoamXAny();
  ~FoamXAny();
  FoamXAny( const FoamXAny& s );
  FoamXAny& operator=( const FoamXAny& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  FoamXType type;
  CORBA::Any value;
};

extern CORBA::TypeCodeConst _tc_FoamXAny;

typedef SequenceTmpl< FoamXAny,MICO_TID_DEF> FoamXAnyList;
typedef TSeqVar< SequenceTmpl< FoamXAny,MICO_TID_DEF> > FoamXAnyList_var;
typedef TSeqOut< SequenceTmpl< FoamXAny,MICO_TID_DEF> > FoamXAnyList_out;

extern CORBA::TypeCodeConst _tc_FoamXAnyList;

struct StringPair;
typedef TVarVar< StringPair > StringPair_var;
typedef TVarOut< StringPair > StringPair_out;


struct StringPair {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef StringPair_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  StringPair();
  ~StringPair();
  StringPair( const StringPair& s );
  StringPair& operator=( const StringPair& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  CORBA::String_var name;
  CORBA::String_var value;
};

extern CORBA::TypeCodeConst _tc_StringPair;

typedef SequenceTmpl< StringPair,MICO_TID_DEF> StringPairList;
typedef TSeqVar< SequenceTmpl< StringPair,MICO_TID_DEF> > StringPairList_var;
typedef TSeqOut< SequenceTmpl< StringPair,MICO_TID_DEF> > StringPairList_out;

extern CORBA::TypeCodeConst _tc_StringPairList;

struct HostDescriptor;
typedef TVarVar< HostDescriptor > HostDescriptor_var;
typedef TVarOut< HostDescriptor > HostDescriptor_out;


struct HostDescriptor {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef HostDescriptor_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  HostDescriptor();
  ~HostDescriptor();
  HostDescriptor( const HostDescriptor& s );
  HostDescriptor& operator=( const HostDescriptor& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  CORBA::String_var name;
  CORBA::Boolean alive;
};

extern CORBA::TypeCodeConst _tc_HostDescriptor;

typedef SequenceTmpl< HostDescriptor,MICO_TID_DEF> HostDescriptorList;
typedef TSeqVar< SequenceTmpl< HostDescriptor,MICO_TID_DEF> > HostDescriptorList_var;
typedef TSeqOut< SequenceTmpl< HostDescriptor,MICO_TID_DEF> > HostDescriptorList_out;

extern CORBA::TypeCodeConst _tc_HostDescriptorList;

struct ApplicationClassDescriptor;
typedef TVarVar< ApplicationClassDescriptor > ApplicationClassDescriptor_var;
typedef TVarOut< ApplicationClassDescriptor > ApplicationClassDescriptor_out;


struct ApplicationClassDescriptor {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef ApplicationClassDescriptor_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  ApplicationClassDescriptor();
  ~ApplicationClassDescriptor();
  ApplicationClassDescriptor( const ApplicationClassDescriptor& s );
  ApplicationClassDescriptor& operator=( const ApplicationClassDescriptor& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  CORBA::String_var name;
  CORBA::String_var category;
  CORBA::Boolean systemClass;
};

extern CORBA::TypeCodeConst _tc_ApplicationClassDescriptor;

typedef SequenceTmpl< ApplicationClassDescriptor,MICO_TID_DEF> ApplicationClassDescriptorList;
typedef TSeqVar< SequenceTmpl< ApplicationClassDescriptor,MICO_TID_DEF> > ApplicationClassDescriptorList_var;
typedef TSeqOut< SequenceTmpl< ApplicationClassDescriptor,MICO_TID_DEF> > ApplicationClassDescriptorList_out;

extern CORBA::TypeCodeConst _tc_ApplicationClassDescriptorList;

struct CaseDescriptor;
typedef TVarVar< CaseDescriptor > CaseDescriptor_var;
typedef TVarOut< CaseDescriptor > CaseDescriptor_out;


struct CaseDescriptor {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef CaseDescriptor_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  CaseDescriptor();
  ~CaseDescriptor();
  CaseDescriptor( const CaseDescriptor& s );
  CaseDescriptor& operator=( const CaseDescriptor& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  CORBA::String_var rootDir;
  CORBA::String_var rawRootDir;
  CORBA::String_var caseName;
  CORBA::String_var appClass;
  CORBA::Long nProcs;
  CORBA::Boolean managed;
  CORBA::Boolean locked;
  CORBA::Boolean error;
};

extern CORBA::TypeCodeConst _tc_CaseDescriptor;

typedef SequenceTmpl< CaseDescriptor,MICO_TID_DEF> CaseDescriptorList;
typedef TSeqVar< SequenceTmpl< CaseDescriptor,MICO_TID_DEF> > CaseDescriptorList_var;
typedef TSeqOut< SequenceTmpl< CaseDescriptor,MICO_TID_DEF> > CaseDescriptorList_out;

extern CORBA::TypeCodeConst _tc_CaseDescriptorList;

struct JobID;
typedef TVarVar< JobID > JobID_var;
typedef TVarOut< JobID > JobID_out;


struct JobID {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef JobID_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  JobID();
  ~JobID();
  JobID( const JobID& s );
  JobID& operator=( const JobID& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  CORBA::String_var hostName;
  CORBA::Long processID;
};

extern CORBA::TypeCodeConst _tc_JobID;

typedef SequenceTmpl< JobID,MICO_TID_DEF> JobIDList;
typedef TSeqVar< SequenceTmpl< JobID,MICO_TID_DEF> > JobIDList_var;
typedef TSeqOut< SequenceTmpl< JobID,MICO_TID_DEF> > JobIDList_out;

extern CORBA::TypeCodeConst _tc_JobIDList;

struct JobDescriptor;
typedef TVarVar< JobDescriptor > JobDescriptor_var;
typedef TVarOut< JobDescriptor > JobDescriptor_out;


struct JobDescriptor {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef JobDescriptor_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  JobDescriptor();
  ~JobDescriptor();
  JobDescriptor( const JobDescriptor& s );
  JobDescriptor& operator=( const JobDescriptor& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  JobID jobID;
  CORBA::Long ppid;
  CORBA::Long pgid;
  CORBA::String_var startDate;
  CORBA::String_var startTime;
  CORBA::String_var userName;
  CORBA::String_var foamVersion;
  CORBA::String_var code;
  CORBA::String_var argList;
  CORBA::String_var currentDir;
  CORBA::String_var rootDir;
  CORBA::String_var caseName;
  CORBA::Long nProcs;
  JobIDList slaves;
  CORBA::Long nCountedProcs;
  CORBA::Double cpuTime;
  CORBA::String_var endDate;
  CORBA::String_var endTime;
  JobStatus status;
};

extern CORBA::TypeCodeConst _tc_JobDescriptor;

typedef SequenceTmpl< JobDescriptor,MICO_TID_DEF> JobDescriptorList;
typedef TSeqVar< SequenceTmpl< JobDescriptor,MICO_TID_DEF> > JobDescriptorList_var;
typedef TSeqOut< SequenceTmpl< JobDescriptor,MICO_TID_DEF> > JobDescriptorList_out;

extern CORBA::TypeCodeConst _tc_JobDescriptorList;

struct DimensionSet;
typedef TFixVar< DimensionSet > DimensionSet_var;
typedef DimensionSet& DimensionSet_out;


struct DimensionSet {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef DimensionSet_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  DimensionSet();
  ~DimensionSet();
  DimensionSet( const DimensionSet& s );
  DimensionSet& operator=( const DimensionSet& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  CORBA::Double mass;
  CORBA::Double length;
  CORBA::Double time;
  CORBA::Double temperature;
  CORBA::Double moles;
  CORBA::Double current;
  CORBA::Double luminousIntensity;
};

extern CORBA::TypeCodeConst _tc_DimensionSet;

typedef SequenceTmpl< DimensionSet,MICO_TID_DEF> DimensionSetList;
typedef TSeqVar< SequenceTmpl< DimensionSet,MICO_TID_DEF> > DimensionSetList_var;
typedef TSeqOut< SequenceTmpl< DimensionSet,MICO_TID_DEF> > DimensionSetList_out;

extern CORBA::TypeCodeConst _tc_DimensionSetList;

struct SurfacePatch;
typedef TVarVar< SurfacePatch > SurfacePatch_var;
typedef TVarOut< SurfacePatch > SurfacePatch_out;


struct SurfacePatch {
  #ifdef HAVE_TYPEDEF_OVERLOAD
  typedef SurfacePatch_var _var_type;
  #endif
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  SurfacePatch();
  ~SurfacePatch();
  SurfacePatch( const SurfacePatch& s );
  SurfacePatch& operator=( const SurfacePatch& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  CORBA::String_var name;
  CORBA::String_var geometricType;
  CORBA::Long size;
  CORBA::Long start;
};

extern CORBA::TypeCodeConst _tc_SurfacePatch;

typedef SequenceTmpl< SurfacePatch,MICO_TID_DEF> SurfacePatchList;
typedef TSeqVar< SequenceTmpl< SurfacePatch,MICO_TID_DEF> > SurfacePatchList_var;
typedef TSeqOut< SequenceTmpl< SurfacePatch,MICO_TID_DEF> > SurfacePatchList_out;

extern CORBA::TypeCodeConst _tc_SurfacePatchList;

struct FoamXError : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  FoamXError();
  ~FoamXError();
  FoamXError( const FoamXError& s );
  FoamXError& operator=( const FoamXError& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  #ifndef HAVE_EXPLICIT_STRUCT_OPS
  FoamXError();
  #endif //HAVE_EXPLICIT_STRUCT_OPS
  FoamXError( ::FoamXServer::ErrorCode _m0, const char* _m1, const char* _m2, const char* _m3, CORBA::Long _m4 );

  #ifdef HAVE_STD_EH
  FoamXError *operator->() { return this; }
  FoamXError& operator*() { return *this; }
  operator FoamXError*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static FoamXError *_downcast( CORBA::Exception *ex );
  static const FoamXError *_downcast( const CORBA::Exception *ex );
  ErrorCode errorCode;
  CORBA::String_var errorMessage;
  CORBA::String_var methodName;
  CORBA::String_var fileName;
  CORBA::Long lineNo;
};

#ifdef HAVE_STD_EH
typedef FoamXError FoamXError_catch;
#else
typedef ExceptVar< FoamXError > FoamXError_var;
typedef TVarOut< FoamXError > FoamXError_out;
typedef FoamXError_var FoamXError_catch;
#endif // HAVE_STD_EH

extern CORBA::TypeCodeConst _tc_FoamXError;

struct FoamXSYSError : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  FoamXSYSError();
  ~FoamXSYSError();
  FoamXSYSError( const FoamXSYSError& s );
  FoamXSYSError& operator=( const FoamXSYSError& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  #ifndef HAVE_EXPLICIT_STRUCT_OPS
  FoamXSYSError();
  #endif //HAVE_EXPLICIT_STRUCT_OPS
  FoamXSYSError( ::FoamXServer::ErrorCode _m0, const char* _m1, const char* _m2, const char* _m3, const char* _m4, CORBA::Long _m5 );

  #ifdef HAVE_STD_EH
  FoamXSYSError *operator->() { return this; }
  FoamXSYSError& operator*() { return *this; }
  operator FoamXSYSError*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static FoamXSYSError *_downcast( CORBA::Exception *ex );
  static const FoamXSYSError *_downcast( const CORBA::Exception *ex );
  ErrorCode errorCode;
  CORBA::String_var errorMessage;
  CORBA::String_var hostName;
  CORBA::String_var methodName;
  CORBA::String_var fileName;
  CORBA::Long lineNo;
};

#ifdef HAVE_STD_EH
typedef FoamXSYSError FoamXSYSError_catch;
#else
typedef ExceptVar< FoamXSYSError > FoamXSYSError_var;
typedef TVarOut< FoamXSYSError > FoamXSYSError_out;
typedef FoamXSYSError_var FoamXSYSError_catch;
#endif // HAVE_STD_EH

extern CORBA::TypeCodeConst _tc_FoamXSYSError;

struct ValidationError : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  ValidationError();
  ~ValidationError();
  ValidationError( const ValidationError& s );
  ValidationError& operator=( const ValidationError& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  #ifndef HAVE_EXPLICIT_STRUCT_OPS
  ValidationError();
  #endif //HAVE_EXPLICIT_STRUCT_OPS
  ValidationError( ::FoamXServer::ErrorCode _m0, const char* _m1, const char* _m2 );

  #ifdef HAVE_STD_EH
  ValidationError *operator->() { return this; }
  ValidationError& operator*() { return *this; }
  operator ValidationError*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static ValidationError *_downcast( CORBA::Exception *ex );
  static const ValidationError *_downcast( const CORBA::Exception *ex );
  ErrorCode errorCode;
  CORBA::String_var errorMessage;
  CORBA::String_var itemPath;
};

#ifdef HAVE_STD_EH
typedef ValidationError ValidationError_catch;
#else
typedef ExceptVar< ValidationError > ValidationError_var;
typedef TVarOut< ValidationError > ValidationError_out;
typedef ValidationError_var ValidationError_catch;
#endif // HAVE_STD_EH

extern CORBA::TypeCodeConst _tc_ValidationError;

struct FoamXIOError : public CORBA::UserException {
  #ifdef HAVE_EXPLICIT_STRUCT_OPS
  FoamXIOError();
  ~FoamXIOError();
  FoamXIOError( const FoamXIOError& s );
  FoamXIOError& operator=( const FoamXIOError& s );
  #endif //HAVE_EXPLICIT_STRUCT_OPS

  #ifndef HAVE_EXPLICIT_STRUCT_OPS
  FoamXIOError();
  #endif //HAVE_EXPLICIT_STRUCT_OPS
  FoamXIOError( const char* _m0, const char* _m1, CORBA::Long _m2, CORBA::Long _m3, const char* _m4, const char* _m5, CORBA::Long _m6 );

  #ifdef HAVE_STD_EH
  FoamXIOError *operator->() { return this; }
  FoamXIOError& operator*() { return *this; }
  operator FoamXIOError*() { return this; }
  #endif // HAVE_STD_EH

  void _throwit() const;
  const char *_repoid() const;
  void _encode( CORBA::DataEncoder &en ) const;
  void _encode_any( CORBA::Any &a ) const;
  CORBA::Exception *_clone() const;
  static FoamXIOError *_downcast( CORBA::Exception *ex );
  static const FoamXIOError *_downcast( const CORBA::Exception *ex );
  CORBA::String_var errorMessage;
  CORBA::String_var ioFileName;
  CORBA::Long ioStartLineNo;
  CORBA::Long ioEndLineNo;
  CORBA::String_var methodName;
  CORBA::String_var fileName;
  CORBA::Long lineNo;
};

#ifdef HAVE_STD_EH
typedef FoamXIOError FoamXIOError_catch;
#else
typedef ExceptVar< FoamXIOError > FoamXIOError_var;
typedef TVarOut< FoamXIOError > FoamXIOError_out;
typedef FoamXIOError_var FoamXIOError_catch;
#endif // HAVE_STD_EH

extern CORBA::TypeCodeConst _tc_FoamXIOError;



namespace PatchToolServer
{


/*
 * Base class and common definitions for interface IPatchToolServer
 */

class IPatchToolServer : 
  virtual public CORBA::Object
{
  public:
    virtual ~IPatchToolServer();

    #ifdef HAVE_TYPEDEF_OVERLOAD
    typedef IPatchToolServer_ptr _ptr_type;
    typedef IPatchToolServer_var _var_type;
    #endif

    static IPatchToolServer_ptr _narrow( CORBA::Object_ptr obj );
    static IPatchToolServer_ptr _narrow( CORBA::AbstractBase_ptr obj );
    static IPatchToolServer_ptr _duplicate( IPatchToolServer_ptr _obj )
    {
      CORBA::Object::_duplicate (_obj);
      return _obj;
    }

    static IPatchToolServer_ptr _nil()
    {
      return 0;
    }

    virtual void *_narrow_helper( const char *repoid );

    virtual char* caseRoot() = 0;
    virtual char* caseName() = 0;
    virtual ::FoamXServer::StringList* availableTimeSteps() = 0;

    virtual char* getTime() = 0;
    virtual void setTime( const char* timeName, CORBA::Long timeIndex ) = 0;
    virtual void read() = 0;
    virtual void readTriSurface( const char* fileName ) = 0;
    virtual void write() = 0;
    virtual void writeTriSurface( const char* fileName ) = 0;
    virtual void getPoints( FloatList_out points ) = 0;
    virtual CORBA::Long getNFaces() = 0;
    virtual CORBA::Long getNEdges() = 0;
    virtual void getEdges( LongList_out verts ) = 0;
    virtual void setFeatureEdges( CORBA::Double minCos ) = 0;
    virtual void getFeaturePoints( FloatList_out points ) = 0;
    virtual void getFeatureEdges( LongList_out edgeLabels ) = 0;
    virtual void getFeatureToEdge( LongList_out edgeLabels ) = 0;
    virtual void getEdgeToFeature( LongList_out featLabels ) = 0;
    virtual void getFeatureSegments( LongLongList_out featureSegments ) = 0;
    virtual void setExtraEdges( CORBA::Long edgeI ) = 0;
    virtual void getExtraEdges( LongList_out edgeLabels ) = 0;
    virtual CORBA::Long whichPatch( CORBA::Long faceI ) = 0;
    virtual CORBA::Long findPatchID( const char* patchName ) = 0;
    virtual void getPatches( SurfacePatchList_out patches ) = 0;
    virtual void addPatch( const char* patchName ) = 0;
    virtual void deletePatch( const char* patchName ) = 0;
    virtual void changePatchType( const char* patchName, const char* patchType ) = 0;
    virtual void changeFaces( const LongList& patchIDs, LongList_out oldToNew ) = 0;
    virtual CORBA::Long getNTris( CORBA::Long startFaceI, CORBA::Long nFaces, LongList_out nTris ) = 0;
    virtual void triangulate( CORBA::Long startFaceI, CORBA::Long nFaces, CORBA::Long totalNTris, LongList_out triVerts ) = 0;
    virtual CORBA::Long getNPoints( CORBA::Long startFaceI, CORBA::Long nFaces ) = 0;
    virtual void triangulateLocal( CORBA::Long startFaceI, CORBA::Long nFaces, CORBA::Long totalNTris, LongList_out triVerts, LongList_out localToGlobal ) = 0;
    virtual void markFaces( const LongList& protectedEdges, CORBA::Long faceI, BoolList_out visited ) = 0;
    virtual void close() = 0;

  protected:
    IPatchToolServer() {};
  private:
    IPatchToolServer( const IPatchToolServer& );
    void operator=( const IPatchToolServer& );
};

extern CORBA::TypeCodeConst _tc_IPatchToolServer;

// Stub for interface IPatchToolServer
class IPatchToolServer_stub:
  virtual public IPatchToolServer
{
  public:
    virtual ~IPatchToolServer_stub();
    char* caseRoot();
    char* caseName();
    ::FoamXServer::StringList* availableTimeSteps();

    char* getTime();
    void setTime( const char* timeName, CORBA::Long timeIndex );
    void read();
    void readTriSurface( const char* fileName );
    void write();
    void writeTriSurface( const char* fileName );
    void getPoints( FloatList_out points );
    CORBA::Long getNFaces();
    CORBA::Long getNEdges();
    void getEdges( LongList_out verts );
    void setFeatureEdges( CORBA::Double minCos );
    void getFeaturePoints( FloatList_out points );
    void getFeatureEdges( LongList_out edgeLabels );
    void getFeatureToEdge( LongList_out edgeLabels );
    void getEdgeToFeature( LongList_out featLabels );
    void getFeatureSegments( LongLongList_out featureSegments );
    void setExtraEdges( CORBA::Long edgeI );
    void getExtraEdges( LongList_out edgeLabels );
    CORBA::Long whichPatch( CORBA::Long faceI );
    CORBA::Long findPatchID( const char* patchName );
    void getPatches( SurfacePatchList_out patches );
    void addPatch( const char* patchName );
    void deletePatch( const char* patchName );
    void changePatchType( const char* patchName, const char* patchType );
    void changeFaces( const LongList& patchIDs, LongList_out oldToNew );
    CORBA::Long getNTris( CORBA::Long startFaceI, CORBA::Long nFaces, LongList_out nTris );
    void triangulate( CORBA::Long startFaceI, CORBA::Long nFaces, CORBA::Long totalNTris, LongList_out triVerts );
    CORBA::Long getNPoints( CORBA::Long startFaceI, CORBA::Long nFaces );
    void triangulateLocal( CORBA::Long startFaceI, CORBA::Long nFaces, CORBA::Long totalNTris, LongList_out triVerts, LongList_out localToGlobal );
    void markFaces( const LongList& protectedEdges, CORBA::Long faceI, BoolList_out visited );
    void close();

  private:
    void operator=( const IPatchToolServer_stub& );
};

#ifndef MICO_CONF_NO_POA

class IPatchToolServer_stub_clp :
  virtual public IPatchToolServer_stub,
  virtual public PortableServer::StubBase
{
  public:
    IPatchToolServer_stub_clp (PortableServer::POA_ptr, CORBA::Object_ptr);
    virtual ~IPatchToolServer_stub_clp ();
    char* caseRoot();
    char* caseName();
    ::FoamXServer::StringList* availableTimeSteps();

    char* getTime();
    void setTime( const char* timeName, CORBA::Long timeIndex );
    void read();
    void readTriSurface( const char* fileName );
    void write();
    void writeTriSurface( const char* fileName );
    void getPoints( FloatList_out points );
    CORBA::Long getNFaces();
    CORBA::Long getNEdges();
    void getEdges( LongList_out verts );
    void setFeatureEdges( CORBA::Double minCos );
    void getFeaturePoints( FloatList_out points );
    void getFeatureEdges( LongList_out edgeLabels );
    void getFeatureToEdge( LongList_out edgeLabels );
    void getEdgeToFeature( LongList_out featLabels );
    void getFeatureSegments( LongLongList_out featureSegments );
    void setExtraEdges( CORBA::Long edgeI );
    void getExtraEdges( LongList_out edgeLabels );
    CORBA::Long whichPatch( CORBA::Long faceI );
    CORBA::Long findPatchID( const char* patchName );
    void getPatches( SurfacePatchList_out patches );
    void addPatch( const char* patchName );
    void deletePatch( const char* patchName );
    void changePatchType( const char* patchName, const char* patchType );
    void changeFaces( const LongList& patchIDs, LongList_out oldToNew );
    CORBA::Long getNTris( CORBA::Long startFaceI, CORBA::Long nFaces, LongList_out nTris );
    void triangulate( CORBA::Long startFaceI, CORBA::Long nFaces, CORBA::Long totalNTris, LongList_out triVerts );
    CORBA::Long getNPoints( CORBA::Long startFaceI, CORBA::Long nFaces );
    void triangulateLocal( CORBA::Long startFaceI, CORBA::Long nFaces, CORBA::Long totalNTris, LongList_out triVerts, LongList_out localToGlobal );
    void markFaces( const LongList& protectedEdges, CORBA::Long faceI, BoolList_out visited );
    void close();

  protected:
    IPatchToolServer_stub_clp ();
  private:
    void operator=( const IPatchToolServer_stub_clp & );
};

#endif // MICO_CONF_NO_POA

}
}


#ifndef MICO_CONF_NO_POA



namespace POA_FoamXServer
{
namespace PatchToolServer
{

class IPatchToolServer : virtual public PortableServer::StaticImplementation
{
  public:
    virtual ~IPatchToolServer ();
    FoamXServer::PatchToolServer::IPatchToolServer_ptr _this ();
    bool dispatch (CORBA::StaticServerRequest_ptr);
    virtual void invoke (CORBA::StaticServerRequest_ptr);
    virtual CORBA::Boolean _is_a (const char *);
    virtual CORBA::InterfaceDef_ptr _get_interface ();
    virtual CORBA::RepositoryId _primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr);

    virtual void * _narrow_helper (const char *);
    static IPatchToolServer * _narrow (PortableServer::Servant);
    virtual CORBA::Object_ptr _make_stub (PortableServer::POA_ptr, CORBA::Object_ptr);

    virtual char* caseRoot() = 0;
    virtual char* caseName() = 0;
    virtual ::FoamXServer::StringList* availableTimeSteps() = 0;

    virtual char* getTime() = 0;
    virtual void setTime( const char* timeName, CORBA::Long timeIndex ) = 0;
    virtual void read() = 0;
    virtual void readTriSurface( const char* fileName ) = 0;
    virtual void write() = 0;
    virtual void writeTriSurface( const char* fileName ) = 0;
    virtual void getPoints( ::FoamXServer::FloatList_out points ) = 0;
    virtual CORBA::Long getNFaces() = 0;
    virtual CORBA::Long getNEdges() = 0;
    virtual void getEdges( ::FoamXServer::LongList_out verts ) = 0;
    virtual void setFeatureEdges( CORBA::Double minCos ) = 0;
    virtual void getFeaturePoints( ::FoamXServer::FloatList_out points ) = 0;
    virtual void getFeatureEdges( ::FoamXServer::LongList_out edgeLabels ) = 0;
    virtual void getFeatureToEdge( ::FoamXServer::LongList_out edgeLabels ) = 0;
    virtual void getEdgeToFeature( ::FoamXServer::LongList_out featLabels ) = 0;
    virtual void getFeatureSegments( ::FoamXServer::LongLongList_out featureSegments ) = 0;
    virtual void setExtraEdges( CORBA::Long edgeI ) = 0;
    virtual void getExtraEdges( ::FoamXServer::LongList_out edgeLabels ) = 0;
    virtual CORBA::Long whichPatch( CORBA::Long faceI ) = 0;
    virtual CORBA::Long findPatchID( const char* patchName ) = 0;
    virtual void getPatches( ::FoamXServer::SurfacePatchList_out patches ) = 0;
    virtual void addPatch( const char* patchName ) = 0;
    virtual void deletePatch( const char* patchName ) = 0;
    virtual void changePatchType( const char* patchName, const char* patchType ) = 0;
    virtual void changeFaces( const ::FoamXServer::LongList& patchIDs, ::FoamXServer::LongList_out oldToNew ) = 0;
    virtual CORBA::Long getNTris( CORBA::Long startFaceI, CORBA::Long nFaces, ::FoamXServer::LongList_out nTris ) = 0;
    virtual void triangulate( CORBA::Long startFaceI, CORBA::Long nFaces, CORBA::Long totalNTris, ::FoamXServer::LongList_out triVerts ) = 0;
    virtual CORBA::Long getNPoints( CORBA::Long startFaceI, CORBA::Long nFaces ) = 0;
    virtual void triangulateLocal( CORBA::Long startFaceI, CORBA::Long nFaces, CORBA::Long totalNTris, ::FoamXServer::LongList_out triVerts, ::FoamXServer::LongList_out localToGlobal ) = 0;
    virtual void markFaces( const ::FoamXServer::LongList& protectedEdges, CORBA::Long faceI, ::FoamXServer::BoolList_out visited ) = 0;
    virtual void close() = 0;

  protected:
    IPatchToolServer () {};

  private:
    IPatchToolServer (const IPatchToolServer &);
    void operator= (const IPatchToolServer &);
};

}
}


#endif // MICO_CONF_NO_POA

void operator<<=( CORBA::Any &a, const ::FoamXServer::FoamXType &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::FoamXServer::FoamXType &e );

extern CORBA::StaticTypeInfo *_marshaller_FoamXServer_FoamXType;

void operator<<=( CORBA::Any &a, const ::FoamXServer::ErrorCode &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::FoamXServer::ErrorCode &e );

extern CORBA::StaticTypeInfo *_marshaller_FoamXServer_ErrorCode;

void operator<<=( CORBA::Any &a, const ::FoamXServer::JobStatus &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::FoamXServer::JobStatus &e );

extern CORBA::StaticTypeInfo *_marshaller_FoamXServer_JobStatus;

void operator<<=( CORBA::Any &a, const ::FoamXServer::MessageType &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, ::FoamXServer::MessageType &e );

extern CORBA::StaticTypeInfo *_marshaller_FoamXServer_MessageType;

void operator<<=( CORBA::Any &_a, const ::FoamXServer::Point3_forany &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::FoamXServer::Point3_forany &_s );
void operator<<=( CORBA::Any &_a, const ::FoamXServer::FoamXAny &_s );
void operator<<=( CORBA::Any &_a, ::FoamXServer::FoamXAny *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::FoamXServer::FoamXAny &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::FoamXServer::FoamXAny *&_s );

extern CORBA::StaticTypeInfo *_marshaller_FoamXServer_FoamXAny;

void operator<<=( CORBA::Any &_a, const ::FoamXServer::StringPair &_s );
void operator<<=( CORBA::Any &_a, ::FoamXServer::StringPair *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::FoamXServer::StringPair &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::FoamXServer::StringPair *&_s );

extern CORBA::StaticTypeInfo *_marshaller_FoamXServer_StringPair;

void operator<<=( CORBA::Any &_a, const ::FoamXServer::HostDescriptor &_s );
void operator<<=( CORBA::Any &_a, ::FoamXServer::HostDescriptor *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::FoamXServer::HostDescriptor &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::FoamXServer::HostDescriptor *&_s );

extern CORBA::StaticTypeInfo *_marshaller_FoamXServer_HostDescriptor;

void operator<<=( CORBA::Any &_a, const ::FoamXServer::ApplicationClassDescriptor &_s );
void operator<<=( CORBA::Any &_a, ::FoamXServer::ApplicationClassDescriptor *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::FoamXServer::ApplicationClassDescriptor &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::FoamXServer::ApplicationClassDescriptor *&_s );

extern CORBA::StaticTypeInfo *_marshaller_FoamXServer_ApplicationClassDescriptor;

void operator<<=( CORBA::Any &_a, const ::FoamXServer::CaseDescriptor &_s );
void operator<<=( CORBA::Any &_a, ::FoamXServer::CaseDescriptor *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::FoamXServer::CaseDescriptor &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::FoamXServer::CaseDescriptor *&_s );

extern CORBA::StaticTypeInfo *_marshaller_FoamXServer_CaseDescriptor;

void operator<<=( CORBA::Any &_a, const ::FoamXServer::JobID &_s );
void operator<<=( CORBA::Any &_a, ::FoamXServer::JobID *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::FoamXServer::JobID &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::FoamXServer::JobID *&_s );

extern CORBA::StaticTypeInfo *_marshaller_FoamXServer_JobID;

void operator<<=( CORBA::Any &_a, const ::FoamXServer::JobDescriptor &_s );
void operator<<=( CORBA::Any &_a, ::FoamXServer::JobDescriptor *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::FoamXServer::JobDescriptor &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::FoamXServer::JobDescriptor *&_s );

extern CORBA::StaticTypeInfo *_marshaller_FoamXServer_JobDescriptor;

void operator<<=( CORBA::Any &_a, const ::FoamXServer::DimensionSet &_s );
void operator<<=( CORBA::Any &_a, ::FoamXServer::DimensionSet *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::FoamXServer::DimensionSet &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::FoamXServer::DimensionSet *&_s );

extern CORBA::StaticTypeInfo *_marshaller_FoamXServer_DimensionSet;

void operator<<=( CORBA::Any &_a, const ::FoamXServer::SurfacePatch &_s );
void operator<<=( CORBA::Any &_a, ::FoamXServer::SurfacePatch *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, ::FoamXServer::SurfacePatch &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const ::FoamXServer::SurfacePatch *&_s );

extern CORBA::StaticTypeInfo *_marshaller_FoamXServer_SurfacePatch;

void operator<<=( CORBA::Any &a, const ::FoamXServer::FoamXError &e );
void operator<<=( CORBA::Any &a, ::FoamXServer::FoamXError *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::FoamXServer::FoamXError &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::FoamXServer::FoamXError *&e );

extern CORBA::StaticTypeInfo *_marshaller_FoamXServer_FoamXError;

void operator<<=( CORBA::Any &a, const ::FoamXServer::FoamXSYSError &e );
void operator<<=( CORBA::Any &a, ::FoamXServer::FoamXSYSError *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::FoamXServer::FoamXSYSError &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::FoamXServer::FoamXSYSError *&e );

extern CORBA::StaticTypeInfo *_marshaller_FoamXServer_FoamXSYSError;

void operator<<=( CORBA::Any &a, const ::FoamXServer::ValidationError &e );
void operator<<=( CORBA::Any &a, ::FoamXServer::ValidationError *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::FoamXServer::ValidationError &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::FoamXServer::ValidationError *&e );

extern CORBA::StaticTypeInfo *_marshaller_FoamXServer_ValidationError;

void operator<<=( CORBA::Any &a, const ::FoamXServer::FoamXIOError &e );
void operator<<=( CORBA::Any &a, ::FoamXServer::FoamXIOError *e );
CORBA::Boolean operator>>=( const CORBA::Any &a, ::FoamXServer::FoamXIOError &e );

CORBA::Boolean operator>>=( const CORBA::Any &a, const ::FoamXServer::FoamXIOError *&e );

extern CORBA::StaticTypeInfo *_marshaller_FoamXServer_FoamXIOError;

void operator<<=( CORBA::Any &a, const FoamXServer::PatchToolServer::IPatchToolServer_ptr obj );
void operator<<=( CORBA::Any &a, FoamXServer::PatchToolServer::IPatchToolServer_ptr* obj_ptr );
CORBA::Boolean operator>>=( const CORBA::Any &a, FoamXServer::PatchToolServer::IPatchToolServer_ptr &obj );

extern CORBA::StaticTypeInfo *_marshaller_FoamXServer_PatchToolServer_IPatchToolServer;

void operator<<=( CORBA::Any &_a, const StringSequenceTmpl<CORBA::String_var> &_s );
void operator<<=( CORBA::Any &_a, StringSequenceTmpl<CORBA::String_var> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, StringSequenceTmpl<CORBA::String_var> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const StringSequenceTmpl<CORBA::String_var> *&_s );

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::Double,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::Double,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::Double,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::Double,MICO_TID_DEF> *&_s );

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::Float,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::Float,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::Float,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::Float,MICO_TID_DEF> *&_s );

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::Boolean,MICO_TID_BOOL> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::Boolean,MICO_TID_BOOL> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::Boolean,MICO_TID_BOOL> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::Boolean,MICO_TID_BOOL> *&_s );

void operator<<=( CORBA::Any &_a, const SequenceTmpl< CORBA::Long,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< CORBA::Long,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< CORBA::Long,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< CORBA::Long,MICO_TID_DEF> *&_s );

void operator<<=( CORBA::Any &_a, const SequenceTmpl< FoamXServer::LongList,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< FoamXServer::LongList,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< FoamXServer::LongList,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< FoamXServer::LongList,MICO_TID_DEF> *&_s );

extern CORBA::StaticTypeInfo *_marshaller__seq__seq_long;

extern CORBA::StaticTypeInfo *_marshaller__a3_float;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< FoamXServer::FoamXAny,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< FoamXServer::FoamXAny,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< FoamXServer::FoamXAny,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< FoamXServer::FoamXAny,MICO_TID_DEF> *&_s );

extern CORBA::StaticTypeInfo *_marshaller__seq_FoamXServer_FoamXAny;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< FoamXServer::StringPair,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< FoamXServer::StringPair,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< FoamXServer::StringPair,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< FoamXServer::StringPair,MICO_TID_DEF> *&_s );

extern CORBA::StaticTypeInfo *_marshaller__seq_FoamXServer_StringPair;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< FoamXServer::HostDescriptor,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< FoamXServer::HostDescriptor,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< FoamXServer::HostDescriptor,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< FoamXServer::HostDescriptor,MICO_TID_DEF> *&_s );

extern CORBA::StaticTypeInfo *_marshaller__seq_FoamXServer_HostDescriptor;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< FoamXServer::ApplicationClassDescriptor,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< FoamXServer::ApplicationClassDescriptor,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< FoamXServer::ApplicationClassDescriptor,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< FoamXServer::ApplicationClassDescriptor,MICO_TID_DEF> *&_s );

extern CORBA::StaticTypeInfo *_marshaller__seq_FoamXServer_ApplicationClassDescriptor;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< FoamXServer::CaseDescriptor,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< FoamXServer::CaseDescriptor,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< FoamXServer::CaseDescriptor,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< FoamXServer::CaseDescriptor,MICO_TID_DEF> *&_s );

extern CORBA::StaticTypeInfo *_marshaller__seq_FoamXServer_CaseDescriptor;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< FoamXServer::JobID,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< FoamXServer::JobID,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< FoamXServer::JobID,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< FoamXServer::JobID,MICO_TID_DEF> *&_s );

extern CORBA::StaticTypeInfo *_marshaller__seq_FoamXServer_JobID;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< FoamXServer::JobDescriptor,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< FoamXServer::JobDescriptor,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< FoamXServer::JobDescriptor,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< FoamXServer::JobDescriptor,MICO_TID_DEF> *&_s );

extern CORBA::StaticTypeInfo *_marshaller__seq_FoamXServer_JobDescriptor;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< FoamXServer::DimensionSet,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< FoamXServer::DimensionSet,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< FoamXServer::DimensionSet,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< FoamXServer::DimensionSet,MICO_TID_DEF> *&_s );

extern CORBA::StaticTypeInfo *_marshaller__seq_FoamXServer_DimensionSet;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< FoamXServer::SurfacePatch,MICO_TID_DEF> &_s );
void operator<<=( CORBA::Any &_a, SequenceTmpl< FoamXServer::SurfacePatch,MICO_TID_DEF> *_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< FoamXServer::SurfacePatch,MICO_TID_DEF> &_s );
CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< FoamXServer::SurfacePatch,MICO_TID_DEF> *&_s );

extern CORBA::StaticTypeInfo *_marshaller__seq_FoamXServer_SurfacePatch;

#endif

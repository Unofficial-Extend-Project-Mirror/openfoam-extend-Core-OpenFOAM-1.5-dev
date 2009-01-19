/*
 *  MICO --- an Open Source CORBA implementation
 *  Copyright (c) 1997-2006 by The Mico Team
 *
 *  This file was automatically generated. DO NOT EDIT!
 */

#include <PatchTool.H>


using namespace std;

//--------------------------------------------------------
//  Implementation of stubs
//--------------------------------------------------------
namespace FoamXServer
{
CORBA::TypeCodeConst _tc_FoamXType;
}

void operator<<=( CORBA::Any &_a, const FoamXServer::FoamXType &_e )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_FoamXType, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, FoamXServer::FoamXType &_e )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_FoamXType, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_FoamXServer_FoamXType : public ::CORBA::StaticTypeInfo {
    typedef FoamXServer::FoamXType _MICO_T;
  public:
    ~_Marshaller_FoamXServer_FoamXType();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_FoamXServer_FoamXType::~_Marshaller_FoamXServer_FoamXType()
{
}

::CORBA::StaticValueType _Marshaller_FoamXServer_FoamXType::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_FoamXServer_FoamXType::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_FoamXServer_FoamXType::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_FoamXServer_FoamXType::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_FoamXServer_FoamXType::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_FoamXServer_FoamXType::typecode()
{
  return FoamXServer::_tc_FoamXType;
}

::CORBA::StaticTypeInfo *_marshaller_FoamXServer_FoamXType;

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_ErrorCode;
}

void operator<<=( CORBA::Any &_a, const FoamXServer::ErrorCode &_e )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_ErrorCode, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, FoamXServer::ErrorCode &_e )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_ErrorCode, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_FoamXServer_ErrorCode : public ::CORBA::StaticTypeInfo {
    typedef FoamXServer::ErrorCode _MICO_T;
  public:
    ~_Marshaller_FoamXServer_ErrorCode();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_FoamXServer_ErrorCode::~_Marshaller_FoamXServer_ErrorCode()
{
}

::CORBA::StaticValueType _Marshaller_FoamXServer_ErrorCode::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_FoamXServer_ErrorCode::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_FoamXServer_ErrorCode::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_FoamXServer_ErrorCode::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_FoamXServer_ErrorCode::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_FoamXServer_ErrorCode::typecode()
{
  return FoamXServer::_tc_ErrorCode;
}

::CORBA::StaticTypeInfo *_marshaller_FoamXServer_ErrorCode;

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_JobStatus;
}

void operator<<=( CORBA::Any &_a, const FoamXServer::JobStatus &_e )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_JobStatus, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, FoamXServer::JobStatus &_e )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_JobStatus, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_FoamXServer_JobStatus : public ::CORBA::StaticTypeInfo {
    typedef FoamXServer::JobStatus _MICO_T;
  public:
    ~_Marshaller_FoamXServer_JobStatus();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_FoamXServer_JobStatus::~_Marshaller_FoamXServer_JobStatus()
{
}

::CORBA::StaticValueType _Marshaller_FoamXServer_JobStatus::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_FoamXServer_JobStatus::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_FoamXServer_JobStatus::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_FoamXServer_JobStatus::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_FoamXServer_JobStatus::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_FoamXServer_JobStatus::typecode()
{
  return FoamXServer::_tc_JobStatus;
}

::CORBA::StaticTypeInfo *_marshaller_FoamXServer_JobStatus;

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_MessageType;
}

void operator<<=( CORBA::Any &_a, const FoamXServer::MessageType &_e )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_MessageType, &_e);
  _a.from_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, FoamXServer::MessageType &_e )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_MessageType, &_e);
  return _a.to_static_any (_sa);
}

class _Marshaller_FoamXServer_MessageType : public ::CORBA::StaticTypeInfo {
    typedef FoamXServer::MessageType _MICO_T;
  public:
    ~_Marshaller_FoamXServer_MessageType();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_FoamXServer_MessageType::~_Marshaller_FoamXServer_MessageType()
{
}

::CORBA::StaticValueType _Marshaller_FoamXServer_MessageType::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_FoamXServer_MessageType::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_FoamXServer_MessageType::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_FoamXServer_MessageType::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong ul;
  if( !dc.enumeration( ul ) )
    return FALSE;
  *(_MICO_T*) v = (_MICO_T) ul;
  return TRUE;
}

void _Marshaller_FoamXServer_MessageType::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.enumeration( (::CORBA::ULong) *(_MICO_T *) v );
}

::CORBA::TypeCode_ptr _Marshaller_FoamXServer_MessageType::typecode()
{
  return FoamXServer::_tc_MessageType;
}

::CORBA::StaticTypeInfo *_marshaller_FoamXServer_MessageType;

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_StringList;
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_DoubleList;
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_FloatList;
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_BoolList;
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_LongList;
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_LongLongList;
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_Point3;
}

void operator<<=( CORBA::Any &_a, const FoamXServer::Point3_forany &_const_s )
{
  _const_s.alloc();
  CORBA::StaticAny _sa (_marshaller__a3_float, _const_s.in());
  _a.from_static_any (_sa);
  _const_s.clear();
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, FoamXServer::Point3_forany &_s )
{
  FoamXServer::Point3_slice *p;
  if (_a.to_static_any (_marshaller__a3_float, (void *&)p)) {
    _s = p;
    return TRUE;
  }
  return FALSE;
}


namespace FoamXServer
{
CORBA::TypeCodeConst _tc_FoamXAny;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
FoamXServer::FoamXAny::FoamXAny()
{
}

FoamXServer::FoamXAny::FoamXAny( const FoamXAny& _s )
{
  type = ((FoamXAny&)_s).type;
  value = ((FoamXAny&)_s).value;
}

FoamXServer::FoamXAny::~FoamXAny()
{
}

FoamXServer::FoamXAny&
FoamXServer::FoamXAny::operator=( const FoamXAny& _s )
{
  type = ((FoamXAny&)_s).type;
  value = ((FoamXAny&)_s).value;
  return *this;
}
#endif

class _Marshaller_FoamXServer_FoamXAny : public ::CORBA::StaticTypeInfo {
    typedef FoamXServer::FoamXAny _MICO_T;
  public:
    ~_Marshaller_FoamXServer_FoamXAny();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_FoamXServer_FoamXAny::~_Marshaller_FoamXServer_FoamXAny()
{
}

::CORBA::StaticValueType _Marshaller_FoamXServer_FoamXAny::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_FoamXServer_FoamXAny::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_FoamXServer_FoamXAny::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_FoamXServer_FoamXAny::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    _marshaller_FoamXServer_FoamXType->demarshal( dc, &((_MICO_T*)v)->type ) &&
    CORBA::_stc_any->demarshal( dc, &((_MICO_T*)v)->value ) &&
    dc.struct_end();
}

void _Marshaller_FoamXServer_FoamXAny::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  _marshaller_FoamXServer_FoamXType->marshal( ec, &((_MICO_T*)v)->type );
  CORBA::_stc_any->marshal( ec, &((_MICO_T*)v)->value );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_FoamXServer_FoamXAny::typecode()
{
  return FoamXServer::_tc_FoamXAny;
}

::CORBA::StaticTypeInfo *_marshaller_FoamXServer_FoamXAny;

void operator<<=( CORBA::Any &_a, const FoamXServer::FoamXAny &_s )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_FoamXAny, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, FoamXServer::FoamXAny *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, FoamXServer::FoamXAny &_s )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_FoamXAny, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const FoamXServer::FoamXAny *&_s )
{
  return _a.to_static_any (_marshaller_FoamXServer_FoamXAny, (void *&)_s);
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_FoamXAnyList;
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_StringPair;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
FoamXServer::StringPair::StringPair()
{
}

FoamXServer::StringPair::StringPair( const StringPair& _s )
{
  name = ((StringPair&)_s).name;
  value = ((StringPair&)_s).value;
}

FoamXServer::StringPair::~StringPair()
{
}

FoamXServer::StringPair&
FoamXServer::StringPair::operator=( const StringPair& _s )
{
  name = ((StringPair&)_s).name;
  value = ((StringPair&)_s).value;
  return *this;
}
#endif

class _Marshaller_FoamXServer_StringPair : public ::CORBA::StaticTypeInfo {
    typedef FoamXServer::StringPair _MICO_T;
  public:
    ~_Marshaller_FoamXServer_StringPair();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_FoamXServer_StringPair::~_Marshaller_FoamXServer_StringPair()
{
}

::CORBA::StaticValueType _Marshaller_FoamXServer_StringPair::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_FoamXServer_StringPair::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_FoamXServer_StringPair::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_FoamXServer_StringPair::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->value._for_demarshal() ) &&
    dc.struct_end();
}

void _Marshaller_FoamXServer_StringPair::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->value.inout() );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_FoamXServer_StringPair::typecode()
{
  return FoamXServer::_tc_StringPair;
}

::CORBA::StaticTypeInfo *_marshaller_FoamXServer_StringPair;

void operator<<=( CORBA::Any &_a, const FoamXServer::StringPair &_s )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_StringPair, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, FoamXServer::StringPair *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, FoamXServer::StringPair &_s )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_StringPair, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const FoamXServer::StringPair *&_s )
{
  return _a.to_static_any (_marshaller_FoamXServer_StringPair, (void *&)_s);
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_StringPairList;
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_HostDescriptor;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
FoamXServer::HostDescriptor::HostDescriptor()
{
}

FoamXServer::HostDescriptor::HostDescriptor( const HostDescriptor& _s )
{
  name = ((HostDescriptor&)_s).name;
  alive = ((HostDescriptor&)_s).alive;
}

FoamXServer::HostDescriptor::~HostDescriptor()
{
}

FoamXServer::HostDescriptor&
FoamXServer::HostDescriptor::operator=( const HostDescriptor& _s )
{
  name = ((HostDescriptor&)_s).name;
  alive = ((HostDescriptor&)_s).alive;
  return *this;
}
#endif

class _Marshaller_FoamXServer_HostDescriptor : public ::CORBA::StaticTypeInfo {
    typedef FoamXServer::HostDescriptor _MICO_T;
  public:
    ~_Marshaller_FoamXServer_HostDescriptor();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_FoamXServer_HostDescriptor::~_Marshaller_FoamXServer_HostDescriptor()
{
}

::CORBA::StaticValueType _Marshaller_FoamXServer_HostDescriptor::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_FoamXServer_HostDescriptor::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_FoamXServer_HostDescriptor::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_FoamXServer_HostDescriptor::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    CORBA::_stc_boolean->demarshal( dc, &((_MICO_T*)v)->alive ) &&
    dc.struct_end();
}

void _Marshaller_FoamXServer_HostDescriptor::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  CORBA::_stc_boolean->marshal( ec, &((_MICO_T*)v)->alive );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_FoamXServer_HostDescriptor::typecode()
{
  return FoamXServer::_tc_HostDescriptor;
}

::CORBA::StaticTypeInfo *_marshaller_FoamXServer_HostDescriptor;

void operator<<=( CORBA::Any &_a, const FoamXServer::HostDescriptor &_s )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_HostDescriptor, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, FoamXServer::HostDescriptor *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, FoamXServer::HostDescriptor &_s )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_HostDescriptor, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const FoamXServer::HostDescriptor *&_s )
{
  return _a.to_static_any (_marshaller_FoamXServer_HostDescriptor, (void *&)_s);
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_HostDescriptorList;
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_ApplicationClassDescriptor;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
FoamXServer::ApplicationClassDescriptor::ApplicationClassDescriptor()
{
}

FoamXServer::ApplicationClassDescriptor::ApplicationClassDescriptor( const ApplicationClassDescriptor& _s )
{
  name = ((ApplicationClassDescriptor&)_s).name;
  category = ((ApplicationClassDescriptor&)_s).category;
  systemClass = ((ApplicationClassDescriptor&)_s).systemClass;
}

FoamXServer::ApplicationClassDescriptor::~ApplicationClassDescriptor()
{
}

FoamXServer::ApplicationClassDescriptor&
FoamXServer::ApplicationClassDescriptor::operator=( const ApplicationClassDescriptor& _s )
{
  name = ((ApplicationClassDescriptor&)_s).name;
  category = ((ApplicationClassDescriptor&)_s).category;
  systemClass = ((ApplicationClassDescriptor&)_s).systemClass;
  return *this;
}
#endif

class _Marshaller_FoamXServer_ApplicationClassDescriptor : public ::CORBA::StaticTypeInfo {
    typedef FoamXServer::ApplicationClassDescriptor _MICO_T;
  public:
    ~_Marshaller_FoamXServer_ApplicationClassDescriptor();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_FoamXServer_ApplicationClassDescriptor::~_Marshaller_FoamXServer_ApplicationClassDescriptor()
{
}

::CORBA::StaticValueType _Marshaller_FoamXServer_ApplicationClassDescriptor::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_FoamXServer_ApplicationClassDescriptor::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_FoamXServer_ApplicationClassDescriptor::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_FoamXServer_ApplicationClassDescriptor::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->category._for_demarshal() ) &&
    CORBA::_stc_boolean->demarshal( dc, &((_MICO_T*)v)->systemClass ) &&
    dc.struct_end();
}

void _Marshaller_FoamXServer_ApplicationClassDescriptor::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->category.inout() );
  CORBA::_stc_boolean->marshal( ec, &((_MICO_T*)v)->systemClass );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_FoamXServer_ApplicationClassDescriptor::typecode()
{
  return FoamXServer::_tc_ApplicationClassDescriptor;
}

::CORBA::StaticTypeInfo *_marshaller_FoamXServer_ApplicationClassDescriptor;

void operator<<=( CORBA::Any &_a, const FoamXServer::ApplicationClassDescriptor &_s )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_ApplicationClassDescriptor, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, FoamXServer::ApplicationClassDescriptor *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, FoamXServer::ApplicationClassDescriptor &_s )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_ApplicationClassDescriptor, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const FoamXServer::ApplicationClassDescriptor *&_s )
{
  return _a.to_static_any (_marshaller_FoamXServer_ApplicationClassDescriptor, (void *&)_s);
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_ApplicationClassDescriptorList;
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_CaseDescriptor;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
FoamXServer::CaseDescriptor::CaseDescriptor()
{
}

FoamXServer::CaseDescriptor::CaseDescriptor( const CaseDescriptor& _s )
{
  rootDir = ((CaseDescriptor&)_s).rootDir;
  rawRootDir = ((CaseDescriptor&)_s).rawRootDir;
  caseName = ((CaseDescriptor&)_s).caseName;
  appClass = ((CaseDescriptor&)_s).appClass;
  nProcs = ((CaseDescriptor&)_s).nProcs;
  managed = ((CaseDescriptor&)_s).managed;
  locked = ((CaseDescriptor&)_s).locked;
  error = ((CaseDescriptor&)_s).error;
}

FoamXServer::CaseDescriptor::~CaseDescriptor()
{
}

FoamXServer::CaseDescriptor&
FoamXServer::CaseDescriptor::operator=( const CaseDescriptor& _s )
{
  rootDir = ((CaseDescriptor&)_s).rootDir;
  rawRootDir = ((CaseDescriptor&)_s).rawRootDir;
  caseName = ((CaseDescriptor&)_s).caseName;
  appClass = ((CaseDescriptor&)_s).appClass;
  nProcs = ((CaseDescriptor&)_s).nProcs;
  managed = ((CaseDescriptor&)_s).managed;
  locked = ((CaseDescriptor&)_s).locked;
  error = ((CaseDescriptor&)_s).error;
  return *this;
}
#endif

class _Marshaller_FoamXServer_CaseDescriptor : public ::CORBA::StaticTypeInfo {
    typedef FoamXServer::CaseDescriptor _MICO_T;
  public:
    ~_Marshaller_FoamXServer_CaseDescriptor();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_FoamXServer_CaseDescriptor::~_Marshaller_FoamXServer_CaseDescriptor()
{
}

::CORBA::StaticValueType _Marshaller_FoamXServer_CaseDescriptor::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_FoamXServer_CaseDescriptor::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_FoamXServer_CaseDescriptor::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_FoamXServer_CaseDescriptor::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->rootDir._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->rawRootDir._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->caseName._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->appClass._for_demarshal() ) &&
    CORBA::_stc_long->demarshal( dc, &((_MICO_T*)v)->nProcs ) &&
    CORBA::_stc_boolean->demarshal( dc, &((_MICO_T*)v)->managed ) &&
    CORBA::_stc_boolean->demarshal( dc, &((_MICO_T*)v)->locked ) &&
    CORBA::_stc_boolean->demarshal( dc, &((_MICO_T*)v)->error ) &&
    dc.struct_end();
}

void _Marshaller_FoamXServer_CaseDescriptor::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->rootDir.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->rawRootDir.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->caseName.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->appClass.inout() );
  CORBA::_stc_long->marshal( ec, &((_MICO_T*)v)->nProcs );
  CORBA::_stc_boolean->marshal( ec, &((_MICO_T*)v)->managed );
  CORBA::_stc_boolean->marshal( ec, &((_MICO_T*)v)->locked );
  CORBA::_stc_boolean->marshal( ec, &((_MICO_T*)v)->error );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_FoamXServer_CaseDescriptor::typecode()
{
  return FoamXServer::_tc_CaseDescriptor;
}

::CORBA::StaticTypeInfo *_marshaller_FoamXServer_CaseDescriptor;

void operator<<=( CORBA::Any &_a, const FoamXServer::CaseDescriptor &_s )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_CaseDescriptor, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, FoamXServer::CaseDescriptor *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, FoamXServer::CaseDescriptor &_s )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_CaseDescriptor, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const FoamXServer::CaseDescriptor *&_s )
{
  return _a.to_static_any (_marshaller_FoamXServer_CaseDescriptor, (void *&)_s);
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_CaseDescriptorList;
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_JobID;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
FoamXServer::JobID::JobID()
{
}

FoamXServer::JobID::JobID( const JobID& _s )
{
  hostName = ((JobID&)_s).hostName;
  processID = ((JobID&)_s).processID;
}

FoamXServer::JobID::~JobID()
{
}

FoamXServer::JobID&
FoamXServer::JobID::operator=( const JobID& _s )
{
  hostName = ((JobID&)_s).hostName;
  processID = ((JobID&)_s).processID;
  return *this;
}
#endif

class _Marshaller_FoamXServer_JobID : public ::CORBA::StaticTypeInfo {
    typedef FoamXServer::JobID _MICO_T;
  public:
    ~_Marshaller_FoamXServer_JobID();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_FoamXServer_JobID::~_Marshaller_FoamXServer_JobID()
{
}

::CORBA::StaticValueType _Marshaller_FoamXServer_JobID::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_FoamXServer_JobID::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_FoamXServer_JobID::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_FoamXServer_JobID::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->hostName._for_demarshal() ) &&
    CORBA::_stc_long->demarshal( dc, &((_MICO_T*)v)->processID ) &&
    dc.struct_end();
}

void _Marshaller_FoamXServer_JobID::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->hostName.inout() );
  CORBA::_stc_long->marshal( ec, &((_MICO_T*)v)->processID );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_FoamXServer_JobID::typecode()
{
  return FoamXServer::_tc_JobID;
}

::CORBA::StaticTypeInfo *_marshaller_FoamXServer_JobID;

void operator<<=( CORBA::Any &_a, const FoamXServer::JobID &_s )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_JobID, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, FoamXServer::JobID *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, FoamXServer::JobID &_s )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_JobID, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const FoamXServer::JobID *&_s )
{
  return _a.to_static_any (_marshaller_FoamXServer_JobID, (void *&)_s);
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_JobIDList;
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_JobDescriptor;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
FoamXServer::JobDescriptor::JobDescriptor()
{
}

FoamXServer::JobDescriptor::JobDescriptor( const JobDescriptor& _s )
{
  jobID = ((JobDescriptor&)_s).jobID;
  ppid = ((JobDescriptor&)_s).ppid;
  pgid = ((JobDescriptor&)_s).pgid;
  startDate = ((JobDescriptor&)_s).startDate;
  startTime = ((JobDescriptor&)_s).startTime;
  userName = ((JobDescriptor&)_s).userName;
  foamVersion = ((JobDescriptor&)_s).foamVersion;
  code = ((JobDescriptor&)_s).code;
  argList = ((JobDescriptor&)_s).argList;
  currentDir = ((JobDescriptor&)_s).currentDir;
  rootDir = ((JobDescriptor&)_s).rootDir;
  caseName = ((JobDescriptor&)_s).caseName;
  nProcs = ((JobDescriptor&)_s).nProcs;
  slaves = ((JobDescriptor&)_s).slaves;
  nCountedProcs = ((JobDescriptor&)_s).nCountedProcs;
  cpuTime = ((JobDescriptor&)_s).cpuTime;
  endDate = ((JobDescriptor&)_s).endDate;
  endTime = ((JobDescriptor&)_s).endTime;
  status = ((JobDescriptor&)_s).status;
}

FoamXServer::JobDescriptor::~JobDescriptor()
{
}

FoamXServer::JobDescriptor&
FoamXServer::JobDescriptor::operator=( const JobDescriptor& _s )
{
  jobID = ((JobDescriptor&)_s).jobID;
  ppid = ((JobDescriptor&)_s).ppid;
  pgid = ((JobDescriptor&)_s).pgid;
  startDate = ((JobDescriptor&)_s).startDate;
  startTime = ((JobDescriptor&)_s).startTime;
  userName = ((JobDescriptor&)_s).userName;
  foamVersion = ((JobDescriptor&)_s).foamVersion;
  code = ((JobDescriptor&)_s).code;
  argList = ((JobDescriptor&)_s).argList;
  currentDir = ((JobDescriptor&)_s).currentDir;
  rootDir = ((JobDescriptor&)_s).rootDir;
  caseName = ((JobDescriptor&)_s).caseName;
  nProcs = ((JobDescriptor&)_s).nProcs;
  slaves = ((JobDescriptor&)_s).slaves;
  nCountedProcs = ((JobDescriptor&)_s).nCountedProcs;
  cpuTime = ((JobDescriptor&)_s).cpuTime;
  endDate = ((JobDescriptor&)_s).endDate;
  endTime = ((JobDescriptor&)_s).endTime;
  status = ((JobDescriptor&)_s).status;
  return *this;
}
#endif

class _Marshaller_FoamXServer_JobDescriptor : public ::CORBA::StaticTypeInfo {
    typedef FoamXServer::JobDescriptor _MICO_T;
  public:
    ~_Marshaller_FoamXServer_JobDescriptor();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_FoamXServer_JobDescriptor::~_Marshaller_FoamXServer_JobDescriptor()
{
}

::CORBA::StaticValueType _Marshaller_FoamXServer_JobDescriptor::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_FoamXServer_JobDescriptor::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_FoamXServer_JobDescriptor::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_FoamXServer_JobDescriptor::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    _marshaller_FoamXServer_JobID->demarshal( dc, &((_MICO_T*)v)->jobID ) &&
    CORBA::_stc_long->demarshal( dc, &((_MICO_T*)v)->ppid ) &&
    CORBA::_stc_long->demarshal( dc, &((_MICO_T*)v)->pgid ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->startDate._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->startTime._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->userName._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->foamVersion._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->code._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->argList._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->currentDir._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->rootDir._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->caseName._for_demarshal() ) &&
    CORBA::_stc_long->demarshal( dc, &((_MICO_T*)v)->nProcs ) &&
    _marshaller__seq_FoamXServer_JobID->demarshal( dc, &((_MICO_T*)v)->slaves ) &&
    CORBA::_stc_long->demarshal( dc, &((_MICO_T*)v)->nCountedProcs ) &&
    CORBA::_stc_double->demarshal( dc, &((_MICO_T*)v)->cpuTime ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->endDate._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->endTime._for_demarshal() ) &&
    _marshaller_FoamXServer_JobStatus->demarshal( dc, &((_MICO_T*)v)->status ) &&
    dc.struct_end();
}

void _Marshaller_FoamXServer_JobDescriptor::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  _marshaller_FoamXServer_JobID->marshal( ec, &((_MICO_T*)v)->jobID );
  CORBA::_stc_long->marshal( ec, &((_MICO_T*)v)->ppid );
  CORBA::_stc_long->marshal( ec, &((_MICO_T*)v)->pgid );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->startDate.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->startTime.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->userName.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->foamVersion.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->code.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->argList.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->currentDir.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->rootDir.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->caseName.inout() );
  CORBA::_stc_long->marshal( ec, &((_MICO_T*)v)->nProcs );
  _marshaller__seq_FoamXServer_JobID->marshal( ec, &((_MICO_T*)v)->slaves );
  CORBA::_stc_long->marshal( ec, &((_MICO_T*)v)->nCountedProcs );
  CORBA::_stc_double->marshal( ec, &((_MICO_T*)v)->cpuTime );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->endDate.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->endTime.inout() );
  _marshaller_FoamXServer_JobStatus->marshal( ec, &((_MICO_T*)v)->status );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_FoamXServer_JobDescriptor::typecode()
{
  return FoamXServer::_tc_JobDescriptor;
}

::CORBA::StaticTypeInfo *_marshaller_FoamXServer_JobDescriptor;

void operator<<=( CORBA::Any &_a, const FoamXServer::JobDescriptor &_s )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_JobDescriptor, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, FoamXServer::JobDescriptor *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, FoamXServer::JobDescriptor &_s )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_JobDescriptor, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const FoamXServer::JobDescriptor *&_s )
{
  return _a.to_static_any (_marshaller_FoamXServer_JobDescriptor, (void *&)_s);
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_JobDescriptorList;
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_DimensionSet;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
FoamXServer::DimensionSet::DimensionSet()
{
}

FoamXServer::DimensionSet::DimensionSet( const DimensionSet& _s )
{
  mass = ((DimensionSet&)_s).mass;
  length = ((DimensionSet&)_s).length;
  time = ((DimensionSet&)_s).time;
  temperature = ((DimensionSet&)_s).temperature;
  moles = ((DimensionSet&)_s).moles;
  current = ((DimensionSet&)_s).current;
  luminousIntensity = ((DimensionSet&)_s).luminousIntensity;
}

FoamXServer::DimensionSet::~DimensionSet()
{
}

FoamXServer::DimensionSet&
FoamXServer::DimensionSet::operator=( const DimensionSet& _s )
{
  mass = ((DimensionSet&)_s).mass;
  length = ((DimensionSet&)_s).length;
  time = ((DimensionSet&)_s).time;
  temperature = ((DimensionSet&)_s).temperature;
  moles = ((DimensionSet&)_s).moles;
  current = ((DimensionSet&)_s).current;
  luminousIntensity = ((DimensionSet&)_s).luminousIntensity;
  return *this;
}
#endif

class _Marshaller_FoamXServer_DimensionSet : public ::CORBA::StaticTypeInfo {
    typedef FoamXServer::DimensionSet _MICO_T;
  public:
    ~_Marshaller_FoamXServer_DimensionSet();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_FoamXServer_DimensionSet::~_Marshaller_FoamXServer_DimensionSet()
{
}

::CORBA::StaticValueType _Marshaller_FoamXServer_DimensionSet::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_FoamXServer_DimensionSet::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_FoamXServer_DimensionSet::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_FoamXServer_DimensionSet::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_double->demarshal( dc, &((_MICO_T*)v)->mass ) &&
    CORBA::_stc_double->demarshal( dc, &((_MICO_T*)v)->length ) &&
    CORBA::_stc_double->demarshal( dc, &((_MICO_T*)v)->time ) &&
    CORBA::_stc_double->demarshal( dc, &((_MICO_T*)v)->temperature ) &&
    CORBA::_stc_double->demarshal( dc, &((_MICO_T*)v)->moles ) &&
    CORBA::_stc_double->demarshal( dc, &((_MICO_T*)v)->current ) &&
    CORBA::_stc_double->demarshal( dc, &((_MICO_T*)v)->luminousIntensity ) &&
    dc.struct_end();
}

void _Marshaller_FoamXServer_DimensionSet::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_double->marshal( ec, &((_MICO_T*)v)->mass );
  CORBA::_stc_double->marshal( ec, &((_MICO_T*)v)->length );
  CORBA::_stc_double->marshal( ec, &((_MICO_T*)v)->time );
  CORBA::_stc_double->marshal( ec, &((_MICO_T*)v)->temperature );
  CORBA::_stc_double->marshal( ec, &((_MICO_T*)v)->moles );
  CORBA::_stc_double->marshal( ec, &((_MICO_T*)v)->current );
  CORBA::_stc_double->marshal( ec, &((_MICO_T*)v)->luminousIntensity );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_FoamXServer_DimensionSet::typecode()
{
  return FoamXServer::_tc_DimensionSet;
}

::CORBA::StaticTypeInfo *_marshaller_FoamXServer_DimensionSet;

void operator<<=( CORBA::Any &_a, const FoamXServer::DimensionSet &_s )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_DimensionSet, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, FoamXServer::DimensionSet *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, FoamXServer::DimensionSet &_s )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_DimensionSet, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const FoamXServer::DimensionSet *&_s )
{
  return _a.to_static_any (_marshaller_FoamXServer_DimensionSet, (void *&)_s);
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_DimensionSetList;
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_SurfacePatch;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
FoamXServer::SurfacePatch::SurfacePatch()
{
}

FoamXServer::SurfacePatch::SurfacePatch( const SurfacePatch& _s )
{
  name = ((SurfacePatch&)_s).name;
  geometricType = ((SurfacePatch&)_s).geometricType;
  size = ((SurfacePatch&)_s).size;
  start = ((SurfacePatch&)_s).start;
}

FoamXServer::SurfacePatch::~SurfacePatch()
{
}

FoamXServer::SurfacePatch&
FoamXServer::SurfacePatch::operator=( const SurfacePatch& _s )
{
  name = ((SurfacePatch&)_s).name;
  geometricType = ((SurfacePatch&)_s).geometricType;
  size = ((SurfacePatch&)_s).size;
  start = ((SurfacePatch&)_s).start;
  return *this;
}
#endif

class _Marshaller_FoamXServer_SurfacePatch : public ::CORBA::StaticTypeInfo {
    typedef FoamXServer::SurfacePatch _MICO_T;
  public:
    ~_Marshaller_FoamXServer_SurfacePatch();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_FoamXServer_SurfacePatch::~_Marshaller_FoamXServer_SurfacePatch()
{
}

::CORBA::StaticValueType _Marshaller_FoamXServer_SurfacePatch::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_FoamXServer_SurfacePatch::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_FoamXServer_SurfacePatch::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_FoamXServer_SurfacePatch::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  return
    dc.struct_begin() &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->name._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->geometricType._for_demarshal() ) &&
    CORBA::_stc_long->demarshal( dc, &((_MICO_T*)v)->size ) &&
    CORBA::_stc_long->demarshal( dc, &((_MICO_T*)v)->start ) &&
    dc.struct_end();
}

void _Marshaller_FoamXServer_SurfacePatch::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.struct_begin();
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->name.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->geometricType.inout() );
  CORBA::_stc_long->marshal( ec, &((_MICO_T*)v)->size );
  CORBA::_stc_long->marshal( ec, &((_MICO_T*)v)->start );
  ec.struct_end();
}

::CORBA::TypeCode_ptr _Marshaller_FoamXServer_SurfacePatch::typecode()
{
  return FoamXServer::_tc_SurfacePatch;
}

::CORBA::StaticTypeInfo *_marshaller_FoamXServer_SurfacePatch;

void operator<<=( CORBA::Any &_a, const FoamXServer::SurfacePatch &_s )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_SurfacePatch, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, FoamXServer::SurfacePatch *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, FoamXServer::SurfacePatch &_s )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_SurfacePatch, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const FoamXServer::SurfacePatch *&_s )
{
  return _a.to_static_any (_marshaller_FoamXServer_SurfacePatch, (void *&)_s);
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_SurfacePatchList;
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_FoamXError;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
FoamXServer::FoamXError::FoamXError()
{
}

FoamXServer::FoamXError::FoamXError( const FoamXError& _s )
{
  errorCode = ((FoamXError&)_s).errorCode;
  errorMessage = ((FoamXError&)_s).errorMessage;
  methodName = ((FoamXError&)_s).methodName;
  fileName = ((FoamXError&)_s).fileName;
  lineNo = ((FoamXError&)_s).lineNo;
}

FoamXServer::FoamXError::~FoamXError()
{
}

FoamXServer::FoamXError&
FoamXServer::FoamXError::operator=( const FoamXError& _s )
{
  errorCode = ((FoamXError&)_s).errorCode;
  errorMessage = ((FoamXError&)_s).errorMessage;
  methodName = ((FoamXError&)_s).methodName;
  fileName = ((FoamXError&)_s).fileName;
  lineNo = ((FoamXError&)_s).lineNo;
  return *this;
}
#endif

#ifndef HAVE_EXPLICIT_STRUCT_OPS
FoamXServer::FoamXError::FoamXError()
{
}

#endif

FoamXServer::FoamXError::FoamXError( FoamXServer::ErrorCode _m0, const char* _m1, const char* _m2, const char* _m3, CORBA::Long _m4 )
{
  errorCode = _m0;
  errorMessage = _m1;
  methodName = _m2;
  fileName = _m3;
  lineNo = _m4;
}

class _Marshaller_FoamXServer_FoamXError : public ::CORBA::StaticTypeInfo {
    typedef ::FoamXServer::FoamXError _MICO_T;
  public:
    ~_Marshaller_FoamXServer_FoamXError();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_FoamXServer_FoamXError::~_Marshaller_FoamXServer_FoamXError()
{
}

::CORBA::StaticValueType _Marshaller_FoamXServer_FoamXError::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_FoamXServer_FoamXError::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_FoamXServer_FoamXError::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_FoamXServer_FoamXError::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    _marshaller_FoamXServer_ErrorCode->demarshal( dc, &((_MICO_T*)v)->errorCode ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->errorMessage._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->methodName._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->fileName._for_demarshal() ) &&
    CORBA::_stc_long->demarshal( dc, &((_MICO_T*)v)->lineNo ) &&
    dc.except_end();
}

void _Marshaller_FoamXServer_FoamXError::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:FoamXServer/FoamXError:1.0" );
  _marshaller_FoamXServer_ErrorCode->marshal( ec, &((_MICO_T*)v)->errorCode );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->errorMessage.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->methodName.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->fileName.inout() );
  CORBA::_stc_long->marshal( ec, &((_MICO_T*)v)->lineNo );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_FoamXServer_FoamXError::typecode()
{
  return FoamXServer::_tc_FoamXError;
}

::CORBA::StaticTypeInfo *_marshaller_FoamXServer_FoamXError;

void operator<<=( CORBA::Any &_a, const FoamXServer::FoamXError &_e )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_FoamXError, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, FoamXServer::FoamXError *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, FoamXServer::FoamXError &_e )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_FoamXError, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const FoamXServer::FoamXError *&_e )
{
  return _a.to_static_any (_marshaller_FoamXServer_FoamXError, (void *&)_e);
}

void FoamXServer::FoamXError::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw FoamXError_var( (FoamXServer::FoamXError*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *FoamXServer::FoamXError::_repoid() const
{
  return "IDL:FoamXServer/FoamXError:1.0";
}

void FoamXServer::FoamXError::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_FoamXServer_FoamXError->marshal( _en, (void*) this );
}

void FoamXServer::FoamXError::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *FoamXServer::FoamXError::_clone() const
{
  return new FoamXError( *this );
}

FoamXServer::FoamXError *FoamXServer::FoamXError::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:FoamXServer/FoamXError:1.0" ) )
    return (FoamXError *) _ex;
  return NULL;
}

const FoamXServer::FoamXError *FoamXServer::FoamXError::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:FoamXServer/FoamXError:1.0" ) )
    return (FoamXError *) _ex;
  return NULL;
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_FoamXSYSError;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
FoamXServer::FoamXSYSError::FoamXSYSError()
{
}

FoamXServer::FoamXSYSError::FoamXSYSError( const FoamXSYSError& _s )
{
  errorCode = ((FoamXSYSError&)_s).errorCode;
  errorMessage = ((FoamXSYSError&)_s).errorMessage;
  hostName = ((FoamXSYSError&)_s).hostName;
  methodName = ((FoamXSYSError&)_s).methodName;
  fileName = ((FoamXSYSError&)_s).fileName;
  lineNo = ((FoamXSYSError&)_s).lineNo;
}

FoamXServer::FoamXSYSError::~FoamXSYSError()
{
}

FoamXServer::FoamXSYSError&
FoamXServer::FoamXSYSError::operator=( const FoamXSYSError& _s )
{
  errorCode = ((FoamXSYSError&)_s).errorCode;
  errorMessage = ((FoamXSYSError&)_s).errorMessage;
  hostName = ((FoamXSYSError&)_s).hostName;
  methodName = ((FoamXSYSError&)_s).methodName;
  fileName = ((FoamXSYSError&)_s).fileName;
  lineNo = ((FoamXSYSError&)_s).lineNo;
  return *this;
}
#endif

#ifndef HAVE_EXPLICIT_STRUCT_OPS
FoamXServer::FoamXSYSError::FoamXSYSError()
{
}

#endif

FoamXServer::FoamXSYSError::FoamXSYSError( FoamXServer::ErrorCode _m0, const char* _m1, const char* _m2, const char* _m3, const char* _m4, CORBA::Long _m5 )
{
  errorCode = _m0;
  errorMessage = _m1;
  hostName = _m2;
  methodName = _m3;
  fileName = _m4;
  lineNo = _m5;
}

class _Marshaller_FoamXServer_FoamXSYSError : public ::CORBA::StaticTypeInfo {
    typedef ::FoamXServer::FoamXSYSError _MICO_T;
  public:
    ~_Marshaller_FoamXServer_FoamXSYSError();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_FoamXServer_FoamXSYSError::~_Marshaller_FoamXServer_FoamXSYSError()
{
}

::CORBA::StaticValueType _Marshaller_FoamXServer_FoamXSYSError::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_FoamXServer_FoamXSYSError::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_FoamXServer_FoamXSYSError::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_FoamXServer_FoamXSYSError::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    _marshaller_FoamXServer_ErrorCode->demarshal( dc, &((_MICO_T*)v)->errorCode ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->errorMessage._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->hostName._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->methodName._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->fileName._for_demarshal() ) &&
    CORBA::_stc_long->demarshal( dc, &((_MICO_T*)v)->lineNo ) &&
    dc.except_end();
}

void _Marshaller_FoamXServer_FoamXSYSError::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:FoamXServer/FoamXSYSError:1.0" );
  _marshaller_FoamXServer_ErrorCode->marshal( ec, &((_MICO_T*)v)->errorCode );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->errorMessage.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->hostName.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->methodName.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->fileName.inout() );
  CORBA::_stc_long->marshal( ec, &((_MICO_T*)v)->lineNo );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_FoamXServer_FoamXSYSError::typecode()
{
  return FoamXServer::_tc_FoamXSYSError;
}

::CORBA::StaticTypeInfo *_marshaller_FoamXServer_FoamXSYSError;

void operator<<=( CORBA::Any &_a, const FoamXServer::FoamXSYSError &_e )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_FoamXSYSError, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, FoamXServer::FoamXSYSError *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, FoamXServer::FoamXSYSError &_e )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_FoamXSYSError, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const FoamXServer::FoamXSYSError *&_e )
{
  return _a.to_static_any (_marshaller_FoamXServer_FoamXSYSError, (void *&)_e);
}

void FoamXServer::FoamXSYSError::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw FoamXSYSError_var( (FoamXServer::FoamXSYSError*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *FoamXServer::FoamXSYSError::_repoid() const
{
  return "IDL:FoamXServer/FoamXSYSError:1.0";
}

void FoamXServer::FoamXSYSError::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_FoamXServer_FoamXSYSError->marshal( _en, (void*) this );
}

void FoamXServer::FoamXSYSError::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *FoamXServer::FoamXSYSError::_clone() const
{
  return new FoamXSYSError( *this );
}

FoamXServer::FoamXSYSError *FoamXServer::FoamXSYSError::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:FoamXServer/FoamXSYSError:1.0" ) )
    return (FoamXSYSError *) _ex;
  return NULL;
}

const FoamXServer::FoamXSYSError *FoamXServer::FoamXSYSError::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:FoamXServer/FoamXSYSError:1.0" ) )
    return (FoamXSYSError *) _ex;
  return NULL;
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_ValidationError;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
FoamXServer::ValidationError::ValidationError()
{
}

FoamXServer::ValidationError::ValidationError( const ValidationError& _s )
{
  errorCode = ((ValidationError&)_s).errorCode;
  errorMessage = ((ValidationError&)_s).errorMessage;
  itemPath = ((ValidationError&)_s).itemPath;
}

FoamXServer::ValidationError::~ValidationError()
{
}

FoamXServer::ValidationError&
FoamXServer::ValidationError::operator=( const ValidationError& _s )
{
  errorCode = ((ValidationError&)_s).errorCode;
  errorMessage = ((ValidationError&)_s).errorMessage;
  itemPath = ((ValidationError&)_s).itemPath;
  return *this;
}
#endif

#ifndef HAVE_EXPLICIT_STRUCT_OPS
FoamXServer::ValidationError::ValidationError()
{
}

#endif

FoamXServer::ValidationError::ValidationError( FoamXServer::ErrorCode _m0, const char* _m1, const char* _m2 )
{
  errorCode = _m0;
  errorMessage = _m1;
  itemPath = _m2;
}

class _Marshaller_FoamXServer_ValidationError : public ::CORBA::StaticTypeInfo {
    typedef ::FoamXServer::ValidationError _MICO_T;
  public:
    ~_Marshaller_FoamXServer_ValidationError();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_FoamXServer_ValidationError::~_Marshaller_FoamXServer_ValidationError()
{
}

::CORBA::StaticValueType _Marshaller_FoamXServer_ValidationError::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_FoamXServer_ValidationError::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_FoamXServer_ValidationError::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_FoamXServer_ValidationError::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    _marshaller_FoamXServer_ErrorCode->demarshal( dc, &((_MICO_T*)v)->errorCode ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->errorMessage._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->itemPath._for_demarshal() ) &&
    dc.except_end();
}

void _Marshaller_FoamXServer_ValidationError::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:FoamXServer/ValidationError:1.0" );
  _marshaller_FoamXServer_ErrorCode->marshal( ec, &((_MICO_T*)v)->errorCode );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->errorMessage.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->itemPath.inout() );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_FoamXServer_ValidationError::typecode()
{
  return FoamXServer::_tc_ValidationError;
}

::CORBA::StaticTypeInfo *_marshaller_FoamXServer_ValidationError;

void operator<<=( CORBA::Any &_a, const FoamXServer::ValidationError &_e )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_ValidationError, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, FoamXServer::ValidationError *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, FoamXServer::ValidationError &_e )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_ValidationError, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const FoamXServer::ValidationError *&_e )
{
  return _a.to_static_any (_marshaller_FoamXServer_ValidationError, (void *&)_e);
}

void FoamXServer::ValidationError::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw ValidationError_var( (FoamXServer::ValidationError*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *FoamXServer::ValidationError::_repoid() const
{
  return "IDL:FoamXServer/ValidationError:1.0";
}

void FoamXServer::ValidationError::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_FoamXServer_ValidationError->marshal( _en, (void*) this );
}

void FoamXServer::ValidationError::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *FoamXServer::ValidationError::_clone() const
{
  return new ValidationError( *this );
}

FoamXServer::ValidationError *FoamXServer::ValidationError::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:FoamXServer/ValidationError:1.0" ) )
    return (ValidationError *) _ex;
  return NULL;
}

const FoamXServer::ValidationError *FoamXServer::ValidationError::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:FoamXServer/ValidationError:1.0" ) )
    return (ValidationError *) _ex;
  return NULL;
}

namespace FoamXServer
{
CORBA::TypeCodeConst _tc_FoamXIOError;
}

#ifdef HAVE_EXPLICIT_STRUCT_OPS
FoamXServer::FoamXIOError::FoamXIOError()
{
}

FoamXServer::FoamXIOError::FoamXIOError( const FoamXIOError& _s )
{
  errorMessage = ((FoamXIOError&)_s).errorMessage;
  ioFileName = ((FoamXIOError&)_s).ioFileName;
  ioStartLineNo = ((FoamXIOError&)_s).ioStartLineNo;
  ioEndLineNo = ((FoamXIOError&)_s).ioEndLineNo;
  methodName = ((FoamXIOError&)_s).methodName;
  fileName = ((FoamXIOError&)_s).fileName;
  lineNo = ((FoamXIOError&)_s).lineNo;
}

FoamXServer::FoamXIOError::~FoamXIOError()
{
}

FoamXServer::FoamXIOError&
FoamXServer::FoamXIOError::operator=( const FoamXIOError& _s )
{
  errorMessage = ((FoamXIOError&)_s).errorMessage;
  ioFileName = ((FoamXIOError&)_s).ioFileName;
  ioStartLineNo = ((FoamXIOError&)_s).ioStartLineNo;
  ioEndLineNo = ((FoamXIOError&)_s).ioEndLineNo;
  methodName = ((FoamXIOError&)_s).methodName;
  fileName = ((FoamXIOError&)_s).fileName;
  lineNo = ((FoamXIOError&)_s).lineNo;
  return *this;
}
#endif

#ifndef HAVE_EXPLICIT_STRUCT_OPS
FoamXServer::FoamXIOError::FoamXIOError()
{
}

#endif

FoamXServer::FoamXIOError::FoamXIOError( const char* _m0, const char* _m1, CORBA::Long _m2, CORBA::Long _m3, const char* _m4, const char* _m5, CORBA::Long _m6 )
{
  errorMessage = _m0;
  ioFileName = _m1;
  ioStartLineNo = _m2;
  ioEndLineNo = _m3;
  methodName = _m4;
  fileName = _m5;
  lineNo = _m6;
}

class _Marshaller_FoamXServer_FoamXIOError : public ::CORBA::StaticTypeInfo {
    typedef ::FoamXServer::FoamXIOError _MICO_T;
  public:
    ~_Marshaller_FoamXServer_FoamXIOError();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_FoamXServer_FoamXIOError::~_Marshaller_FoamXServer_FoamXIOError()
{
}

::CORBA::StaticValueType _Marshaller_FoamXServer_FoamXIOError::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller_FoamXServer_FoamXIOError::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller_FoamXServer_FoamXIOError::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller_FoamXServer_FoamXIOError::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  string repoid;
  return
    dc.except_begin( repoid ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->errorMessage._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->ioFileName._for_demarshal() ) &&
    CORBA::_stc_long->demarshal( dc, &((_MICO_T*)v)->ioStartLineNo ) &&
    CORBA::_stc_long->demarshal( dc, &((_MICO_T*)v)->ioEndLineNo ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->methodName._for_demarshal() ) &&
    CORBA::_stc_string->demarshal( dc, &((_MICO_T*)v)->fileName._for_demarshal() ) &&
    CORBA::_stc_long->demarshal( dc, &((_MICO_T*)v)->lineNo ) &&
    dc.except_end();
}

void _Marshaller_FoamXServer_FoamXIOError::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.except_begin( "IDL:FoamXServer/FoamXIOError:1.0" );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->errorMessage.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->ioFileName.inout() );
  CORBA::_stc_long->marshal( ec, &((_MICO_T*)v)->ioStartLineNo );
  CORBA::_stc_long->marshal( ec, &((_MICO_T*)v)->ioEndLineNo );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->methodName.inout() );
  CORBA::_stc_string->marshal( ec, &((_MICO_T*)v)->fileName.inout() );
  CORBA::_stc_long->marshal( ec, &((_MICO_T*)v)->lineNo );
  ec.except_end();
}

::CORBA::TypeCode_ptr _Marshaller_FoamXServer_FoamXIOError::typecode()
{
  return FoamXServer::_tc_FoamXIOError;
}

::CORBA::StaticTypeInfo *_marshaller_FoamXServer_FoamXIOError;

void operator<<=( CORBA::Any &_a, const FoamXServer::FoamXIOError &_e )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_FoamXIOError, &_e);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, FoamXServer::FoamXIOError *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, FoamXServer::FoamXIOError &_e )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_FoamXIOError, &_e);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const FoamXServer::FoamXIOError *&_e )
{
  return _a.to_static_any (_marshaller_FoamXServer_FoamXIOError, (void *&)_e);
}

void FoamXServer::FoamXIOError::_throwit() const
{
  #ifdef HAVE_EXCEPTIONS
  #ifdef HAVE_STD_EH
  throw *this;
  #else
  throw FoamXIOError_var( (FoamXServer::FoamXIOError*)_clone() );
  #endif
  #else
  CORBA::Exception::_throw_failed( _clone() );
  #endif
}

const char *FoamXServer::FoamXIOError::_repoid() const
{
  return "IDL:FoamXServer/FoamXIOError:1.0";
}

void FoamXServer::FoamXIOError::_encode( CORBA::DataEncoder &_en ) const
{
  _marshaller_FoamXServer_FoamXIOError->marshal( _en, (void*) this );
}

void FoamXServer::FoamXIOError::_encode_any( CORBA::Any &_a ) const
{
  _a <<= *this;
}

CORBA::Exception *FoamXServer::FoamXIOError::_clone() const
{
  return new FoamXIOError( *this );
}

FoamXServer::FoamXIOError *FoamXServer::FoamXIOError::_downcast( CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:FoamXServer/FoamXIOError:1.0" ) )
    return (FoamXIOError *) _ex;
  return NULL;
}

const FoamXServer::FoamXIOError *FoamXServer::FoamXIOError::_downcast( const CORBA::Exception *_ex )
{
  if( _ex && !strcmp( _ex->_repoid(), "IDL:FoamXServer/FoamXIOError:1.0" ) )
    return (FoamXIOError *) _ex;
  return NULL;
}


/*
 * Base interface for class IPatchToolServer
 */

FoamXServer::PatchToolServer::IPatchToolServer::~IPatchToolServer()
{
}

void *
FoamXServer::PatchToolServer::IPatchToolServer::_narrow_helper( const char *_repoid )
{
  if( strcmp( _repoid, "IDL:FoamXServer/PatchToolServer/IPatchToolServer:1.0" ) == 0 )
    return (void *)this;
  return NULL;
}

FoamXServer::PatchToolServer::IPatchToolServer_ptr
FoamXServer::PatchToolServer::IPatchToolServer::_narrow( CORBA::Object_ptr _obj )
{
  FoamXServer::PatchToolServer::IPatchToolServer_ptr _o;
  if( !CORBA::is_nil( _obj ) ) {
    void *_p;
    if( (_p = _obj->_narrow_helper( "IDL:FoamXServer/PatchToolServer/IPatchToolServer:1.0" )))
      return _duplicate( (FoamXServer::PatchToolServer::IPatchToolServer_ptr) _p );
    if (!strcmp (_obj->_repoid(), "IDL:FoamXServer/PatchToolServer/IPatchToolServer:1.0") || _obj->_is_a_remote ("IDL:FoamXServer/PatchToolServer/IPatchToolServer:1.0")) {
      _o = new FoamXServer::PatchToolServer::IPatchToolServer_stub;
      _o->CORBA::Object::operator=( *_obj );
      return _o;
    }
  }
  return _nil();
}

FoamXServer::PatchToolServer::IPatchToolServer_ptr
FoamXServer::PatchToolServer::IPatchToolServer::_narrow( CORBA::AbstractBase_ptr _obj )
{
  return _narrow (_obj->_to_object());
}

namespace FoamXServer
{
namespace PatchToolServer
{
CORBA::TypeCodeConst _tc_IPatchToolServer;
}
}
class _Marshaller_FoamXServer_PatchToolServer_IPatchToolServer : public ::CORBA::StaticTypeInfo {
    typedef FoamXServer::PatchToolServer::IPatchToolServer_ptr _MICO_T;
  public:
    ~_Marshaller_FoamXServer_PatchToolServer_IPatchToolServer();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    void release (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller_FoamXServer_PatchToolServer_IPatchToolServer::~_Marshaller_FoamXServer_PatchToolServer_IPatchToolServer()
{
}

::CORBA::StaticValueType _Marshaller_FoamXServer_PatchToolServer_IPatchToolServer::create() const
{
  return (StaticValueType) new _MICO_T( 0 );
}

void _Marshaller_FoamXServer_PatchToolServer_IPatchToolServer::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = ::FoamXServer::PatchToolServer::IPatchToolServer::_duplicate( *(_MICO_T*) s );
}

void _Marshaller_FoamXServer_PatchToolServer_IPatchToolServer::free( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
  delete (_MICO_T*) v;
}

void _Marshaller_FoamXServer_PatchToolServer_IPatchToolServer::release( StaticValueType v ) const
{
  ::CORBA::release( *(_MICO_T *) v );
}

::CORBA::Boolean _Marshaller_FoamXServer_PatchToolServer_IPatchToolServer::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj;
  if (!::CORBA::_stc_Object->demarshal(dc, &obj))
    return FALSE;
  *(_MICO_T *) v = ::FoamXServer::PatchToolServer::IPatchToolServer::_narrow( obj );
  ::CORBA::Boolean ret = ::CORBA::is_nil (obj) || !::CORBA::is_nil (*(_MICO_T *)v);
  ::CORBA::release (obj);
  return ret;
}

void _Marshaller_FoamXServer_PatchToolServer_IPatchToolServer::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::Object_ptr obj = *(_MICO_T *) v;
  ::CORBA::_stc_Object->marshal( ec, &obj );
}

::CORBA::TypeCode_ptr _Marshaller_FoamXServer_PatchToolServer_IPatchToolServer::typecode()
{
  return FoamXServer::PatchToolServer::_tc_IPatchToolServer;
}

::CORBA::StaticTypeInfo *_marshaller_FoamXServer_PatchToolServer_IPatchToolServer;

void
operator<<=( CORBA::Any &_a, const FoamXServer::PatchToolServer::IPatchToolServer_ptr _obj )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_PatchToolServer_IPatchToolServer, &_obj);
  _a.from_static_any (_sa);
}

void
operator<<=( CORBA::Any &_a, FoamXServer::PatchToolServer::IPatchToolServer_ptr* _obj_ptr )
{
  CORBA::StaticAny _sa (_marshaller_FoamXServer_PatchToolServer_IPatchToolServer, _obj_ptr);
  _a.from_static_any (_sa);
  CORBA::release (*_obj_ptr);
}

CORBA::Boolean
operator>>=( const CORBA::Any &_a, FoamXServer::PatchToolServer::IPatchToolServer_ptr &_obj )
{
  FoamXServer::PatchToolServer::IPatchToolServer_ptr *p;
  if (_a.to_static_any (_marshaller_FoamXServer_PatchToolServer_IPatchToolServer, (void *&)p)) {
    _obj = *p;
    return TRUE;
  }
  return FALSE;
}


/*
 * Stub interface for class IPatchToolServer
 */

FoamXServer::PatchToolServer::IPatchToolServer_stub::~IPatchToolServer_stub()
{
}

#ifndef MICO_CONF_NO_POA

void *
POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow_helper (const char * repoid)
{
  if (strcmp (repoid, "IDL:FoamXServer/PatchToolServer/IPatchToolServer:1.0") == 0) {
    return (void *) this;
  }
  return NULL;
}

POA_FoamXServer::PatchToolServer::IPatchToolServer *
POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (PortableServer::Servant serv) 
{
  void * p;
  if ((p = serv->_narrow_helper ("IDL:FoamXServer/PatchToolServer/IPatchToolServer:1.0")) != NULL) {
    serv->_add_ref ();
    return (POA_FoamXServer::PatchToolServer::IPatchToolServer *) p;
  }
  return NULL;
}

FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::IPatchToolServer_stub_clp ()
{
}

FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::IPatchToolServer_stub_clp (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
  : CORBA::Object(*obj), PortableServer::StubBase(poa)
{
}

FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::~IPatchToolServer_stub_clp ()
{
}

#endif // MICO_CONF_NO_POA

char* FoamXServer::PatchToolServer::IPatchToolServer_stub::caseRoot()
{
  char* _res = NULL;
  CORBA::StaticAny __res( CORBA::_stc_string, &_res );

  CORBA::StaticRequest __req( this, "_get_caseRoot" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

char*
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::caseRoot()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      char* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->caseRoot();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return FoamXServer::PatchToolServer::IPatchToolServer_stub::caseRoot();
}

#endif // MICO_CONF_NO_POA

char* FoamXServer::PatchToolServer::IPatchToolServer_stub::caseName()
{
  char* _res = NULL;
  CORBA::StaticAny __res( CORBA::_stc_string, &_res );

  CORBA::StaticRequest __req( this, "_get_caseName" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

char*
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::caseName()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      char* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->caseName();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return FoamXServer::PatchToolServer::IPatchToolServer_stub::caseName();
}

#endif // MICO_CONF_NO_POA

FoamXServer::StringList* FoamXServer::PatchToolServer::IPatchToolServer_stub::availableTimeSteps()
{
  CORBA::StaticAny __res( CORBA::_stcseq_string );

  CORBA::StaticRequest __req( this, "_get_availableTimeSteps" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    0);
  return (FoamXServer::StringList*) __res._retn();
}


#ifndef MICO_CONF_NO_POA

FoamXServer::StringList*
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::availableTimeSteps()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      FoamXServer::StringList* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->availableTimeSteps();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return FoamXServer::PatchToolServer::IPatchToolServer_stub::availableTimeSteps();
}

#endif // MICO_CONF_NO_POA

char* FoamXServer::PatchToolServer::IPatchToolServer_stub::getTime()
{
  char* _res = NULL;
  CORBA::StaticAny __res( CORBA::_stc_string, &_res );

  CORBA::StaticRequest __req( this, "getTime" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    _marshaller_FoamXServer_FoamXIOError, "IDL:FoamXServer/FoamXIOError:1.0",
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

char*
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::getTime()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      char* __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->getTime();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return FoamXServer::PatchToolServer::IPatchToolServer_stub::getTime();
}

#endif // MICO_CONF_NO_POA

void FoamXServer::PatchToolServer::IPatchToolServer_stub::setTime( const char* _par_timeName, CORBA::Long _par_timeIndex )
{
  CORBA::StaticAny _sa_timeName( CORBA::_stc_string, &_par_timeName );
  CORBA::StaticAny _sa_timeIndex( CORBA::_stc_long, &_par_timeIndex );
  CORBA::StaticRequest __req( this, "setTime" );
  __req.add_in_arg( &_sa_timeName );
  __req.add_in_arg( &_sa_timeIndex );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    _marshaller_FoamXServer_FoamXIOError, "IDL:FoamXServer/FoamXIOError:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::setTime( const char* _par_timeName, CORBA::Long _par_timeIndex )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->setTime(_par_timeName, _par_timeIndex);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  FoamXServer::PatchToolServer::IPatchToolServer_stub::setTime(_par_timeName, _par_timeIndex);
}

#endif // MICO_CONF_NO_POA

void FoamXServer::PatchToolServer::IPatchToolServer_stub::read()
{
  CORBA::StaticRequest __req( this, "read" );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    _marshaller_FoamXServer_FoamXIOError, "IDL:FoamXServer/FoamXIOError:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::read()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->read();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  FoamXServer::PatchToolServer::IPatchToolServer_stub::read();
}

#endif // MICO_CONF_NO_POA

void FoamXServer::PatchToolServer::IPatchToolServer_stub::readTriSurface( const char* _par_fileName )
{
  CORBA::StaticAny _sa_fileName( CORBA::_stc_string, &_par_fileName );
  CORBA::StaticRequest __req( this, "readTriSurface" );
  __req.add_in_arg( &_sa_fileName );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    _marshaller_FoamXServer_FoamXIOError, "IDL:FoamXServer/FoamXIOError:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::readTriSurface( const char* _par_fileName )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->readTriSurface(_par_fileName);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  FoamXServer::PatchToolServer::IPatchToolServer_stub::readTriSurface(_par_fileName);
}

#endif // MICO_CONF_NO_POA

void FoamXServer::PatchToolServer::IPatchToolServer_stub::write()
{
  CORBA::StaticRequest __req( this, "write" );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    _marshaller_FoamXServer_FoamXIOError, "IDL:FoamXServer/FoamXIOError:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::write()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->write();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  FoamXServer::PatchToolServer::IPatchToolServer_stub::write();
}

#endif // MICO_CONF_NO_POA

void FoamXServer::PatchToolServer::IPatchToolServer_stub::writeTriSurface( const char* _par_fileName )
{
  CORBA::StaticAny _sa_fileName( CORBA::_stc_string, &_par_fileName );
  CORBA::StaticRequest __req( this, "writeTriSurface" );
  __req.add_in_arg( &_sa_fileName );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    _marshaller_FoamXServer_FoamXIOError, "IDL:FoamXServer/FoamXIOError:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::writeTriSurface( const char* _par_fileName )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->writeTriSurface(_par_fileName);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  FoamXServer::PatchToolServer::IPatchToolServer_stub::writeTriSurface(_par_fileName);
}

#endif // MICO_CONF_NO_POA

void FoamXServer::PatchToolServer::IPatchToolServer_stub::getPoints( FoamXServer::FloatList_out _par_points )
{
  CORBA::StaticAny _sa_points( CORBA::_stcseq_float );
  CORBA::StaticRequest __req( this, "getPoints" );
  __req.add_out_arg( &_sa_points );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    0);
  _par_points = (FoamXServer::FloatList*) _sa_points._retn();
}


#ifndef MICO_CONF_NO_POA

void
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::getPoints( FoamXServer::FloatList_out _par_points )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->getPoints(_par_points);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  FoamXServer::PatchToolServer::IPatchToolServer_stub::getPoints(_par_points);
}

#endif // MICO_CONF_NO_POA

CORBA::Long FoamXServer::PatchToolServer::IPatchToolServer_stub::getNFaces()
{
  CORBA::Long _res;
  CORBA::StaticAny __res( CORBA::_stc_long, &_res );

  CORBA::StaticRequest __req( this, "getNFaces" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Long
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::getNFaces()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      CORBA::Long __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->getNFaces();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return FoamXServer::PatchToolServer::IPatchToolServer_stub::getNFaces();
}

#endif // MICO_CONF_NO_POA

CORBA::Long FoamXServer::PatchToolServer::IPatchToolServer_stub::getNEdges()
{
  CORBA::Long _res;
  CORBA::StaticAny __res( CORBA::_stc_long, &_res );

  CORBA::StaticRequest __req( this, "getNEdges" );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Long
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::getNEdges()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      CORBA::Long __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->getNEdges();
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return FoamXServer::PatchToolServer::IPatchToolServer_stub::getNEdges();
}

#endif // MICO_CONF_NO_POA

void FoamXServer::PatchToolServer::IPatchToolServer_stub::getEdges( FoamXServer::LongList_out _par_verts )
{
  CORBA::StaticAny _sa_verts( CORBA::_stcseq_long );
  CORBA::StaticRequest __req( this, "getEdges" );
  __req.add_out_arg( &_sa_verts );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    0);
  _par_verts = (FoamXServer::LongList*) _sa_verts._retn();
}


#ifndef MICO_CONF_NO_POA

void
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::getEdges( FoamXServer::LongList_out _par_verts )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->getEdges(_par_verts);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  FoamXServer::PatchToolServer::IPatchToolServer_stub::getEdges(_par_verts);
}

#endif // MICO_CONF_NO_POA

void FoamXServer::PatchToolServer::IPatchToolServer_stub::setFeatureEdges( CORBA::Double _par_minCos )
{
  CORBA::StaticAny _sa_minCos( CORBA::_stc_double, &_par_minCos );
  CORBA::StaticRequest __req( this, "setFeatureEdges" );
  __req.add_in_arg( &_sa_minCos );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::setFeatureEdges( CORBA::Double _par_minCos )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->setFeatureEdges(_par_minCos);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  FoamXServer::PatchToolServer::IPatchToolServer_stub::setFeatureEdges(_par_minCos);
}

#endif // MICO_CONF_NO_POA

void FoamXServer::PatchToolServer::IPatchToolServer_stub::getFeaturePoints( FoamXServer::FloatList_out _par_points )
{
  CORBA::StaticAny _sa_points( CORBA::_stcseq_float );
  CORBA::StaticRequest __req( this, "getFeaturePoints" );
  __req.add_out_arg( &_sa_points );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    0);
  _par_points = (FoamXServer::FloatList*) _sa_points._retn();
}


#ifndef MICO_CONF_NO_POA

void
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::getFeaturePoints( FoamXServer::FloatList_out _par_points )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->getFeaturePoints(_par_points);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  FoamXServer::PatchToolServer::IPatchToolServer_stub::getFeaturePoints(_par_points);
}

#endif // MICO_CONF_NO_POA

void FoamXServer::PatchToolServer::IPatchToolServer_stub::getFeatureEdges( FoamXServer::LongList_out _par_edgeLabels )
{
  CORBA::StaticAny _sa_edgeLabels( CORBA::_stcseq_long );
  CORBA::StaticRequest __req( this, "getFeatureEdges" );
  __req.add_out_arg( &_sa_edgeLabels );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    0);
  _par_edgeLabels = (FoamXServer::LongList*) _sa_edgeLabels._retn();
}


#ifndef MICO_CONF_NO_POA

void
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::getFeatureEdges( FoamXServer::LongList_out _par_edgeLabels )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->getFeatureEdges(_par_edgeLabels);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  FoamXServer::PatchToolServer::IPatchToolServer_stub::getFeatureEdges(_par_edgeLabels);
}

#endif // MICO_CONF_NO_POA

void FoamXServer::PatchToolServer::IPatchToolServer_stub::getFeatureToEdge( FoamXServer::LongList_out _par_edgeLabels )
{
  CORBA::StaticAny _sa_edgeLabels( CORBA::_stcseq_long );
  CORBA::StaticRequest __req( this, "getFeatureToEdge" );
  __req.add_out_arg( &_sa_edgeLabels );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    0);
  _par_edgeLabels = (FoamXServer::LongList*) _sa_edgeLabels._retn();
}


#ifndef MICO_CONF_NO_POA

void
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::getFeatureToEdge( FoamXServer::LongList_out _par_edgeLabels )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->getFeatureToEdge(_par_edgeLabels);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  FoamXServer::PatchToolServer::IPatchToolServer_stub::getFeatureToEdge(_par_edgeLabels);
}

#endif // MICO_CONF_NO_POA

void FoamXServer::PatchToolServer::IPatchToolServer_stub::getEdgeToFeature( FoamXServer::LongList_out _par_featLabels )
{
  CORBA::StaticAny _sa_featLabels( CORBA::_stcseq_long );
  CORBA::StaticRequest __req( this, "getEdgeToFeature" );
  __req.add_out_arg( &_sa_featLabels );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    0);
  _par_featLabels = (FoamXServer::LongList*) _sa_featLabels._retn();
}


#ifndef MICO_CONF_NO_POA

void
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::getEdgeToFeature( FoamXServer::LongList_out _par_featLabels )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->getEdgeToFeature(_par_featLabels);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  FoamXServer::PatchToolServer::IPatchToolServer_stub::getEdgeToFeature(_par_featLabels);
}

#endif // MICO_CONF_NO_POA

void FoamXServer::PatchToolServer::IPatchToolServer_stub::getFeatureSegments( FoamXServer::LongLongList_out _par_featureSegments )
{
  CORBA::StaticAny _sa_featureSegments( _marshaller__seq__seq_long );
  CORBA::StaticRequest __req( this, "getFeatureSegments" );
  __req.add_out_arg( &_sa_featureSegments );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    0);
  _par_featureSegments = (FoamXServer::LongLongList*) _sa_featureSegments._retn();
}


#ifndef MICO_CONF_NO_POA

void
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::getFeatureSegments( FoamXServer::LongLongList_out _par_featureSegments )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->getFeatureSegments(_par_featureSegments);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  FoamXServer::PatchToolServer::IPatchToolServer_stub::getFeatureSegments(_par_featureSegments);
}

#endif // MICO_CONF_NO_POA

void FoamXServer::PatchToolServer::IPatchToolServer_stub::setExtraEdges( CORBA::Long _par_edgeI )
{
  CORBA::StaticAny _sa_edgeI( CORBA::_stc_long, &_par_edgeI );
  CORBA::StaticRequest __req( this, "setExtraEdges" );
  __req.add_in_arg( &_sa_edgeI );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::setExtraEdges( CORBA::Long _par_edgeI )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->setExtraEdges(_par_edgeI);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  FoamXServer::PatchToolServer::IPatchToolServer_stub::setExtraEdges(_par_edgeI);
}

#endif // MICO_CONF_NO_POA

void FoamXServer::PatchToolServer::IPatchToolServer_stub::getExtraEdges( FoamXServer::LongList_out _par_edgeLabels )
{
  CORBA::StaticAny _sa_edgeLabels( CORBA::_stcseq_long );
  CORBA::StaticRequest __req( this, "getExtraEdges" );
  __req.add_out_arg( &_sa_edgeLabels );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    0);
  _par_edgeLabels = (FoamXServer::LongList*) _sa_edgeLabels._retn();
}


#ifndef MICO_CONF_NO_POA

void
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::getExtraEdges( FoamXServer::LongList_out _par_edgeLabels )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->getExtraEdges(_par_edgeLabels);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  FoamXServer::PatchToolServer::IPatchToolServer_stub::getExtraEdges(_par_edgeLabels);
}

#endif // MICO_CONF_NO_POA

CORBA::Long FoamXServer::PatchToolServer::IPatchToolServer_stub::whichPatch( CORBA::Long _par_faceI )
{
  CORBA::StaticAny _sa_faceI( CORBA::_stc_long, &_par_faceI );
  CORBA::Long _res;
  CORBA::StaticAny __res( CORBA::_stc_long, &_res );

  CORBA::StaticRequest __req( this, "whichPatch" );
  __req.add_in_arg( &_sa_faceI );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Long
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::whichPatch( CORBA::Long _par_faceI )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      CORBA::Long __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->whichPatch(_par_faceI);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return FoamXServer::PatchToolServer::IPatchToolServer_stub::whichPatch(_par_faceI);
}

#endif // MICO_CONF_NO_POA

CORBA::Long FoamXServer::PatchToolServer::IPatchToolServer_stub::findPatchID( const char* _par_patchName )
{
  CORBA::StaticAny _sa_patchName( CORBA::_stc_string, &_par_patchName );
  CORBA::Long _res;
  CORBA::StaticAny __res( CORBA::_stc_long, &_res );

  CORBA::StaticRequest __req( this, "findPatchID" );
  __req.add_in_arg( &_sa_patchName );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Long
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::findPatchID( const char* _par_patchName )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      CORBA::Long __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->findPatchID(_par_patchName);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return FoamXServer::PatchToolServer::IPatchToolServer_stub::findPatchID(_par_patchName);
}

#endif // MICO_CONF_NO_POA

void FoamXServer::PatchToolServer::IPatchToolServer_stub::getPatches( FoamXServer::SurfacePatchList_out _par_patches )
{
  CORBA::StaticAny _sa_patches( _marshaller__seq_FoamXServer_SurfacePatch );
  CORBA::StaticRequest __req( this, "getPatches" );
  __req.add_out_arg( &_sa_patches );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    0);
  _par_patches = (FoamXServer::SurfacePatchList*) _sa_patches._retn();
}


#ifndef MICO_CONF_NO_POA

void
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::getPatches( FoamXServer::SurfacePatchList_out _par_patches )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->getPatches(_par_patches);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  FoamXServer::PatchToolServer::IPatchToolServer_stub::getPatches(_par_patches);
}

#endif // MICO_CONF_NO_POA

void FoamXServer::PatchToolServer::IPatchToolServer_stub::addPatch( const char* _par_patchName )
{
  CORBA::StaticAny _sa_patchName( CORBA::_stc_string, &_par_patchName );
  CORBA::StaticRequest __req( this, "addPatch" );
  __req.add_in_arg( &_sa_patchName );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::addPatch( const char* _par_patchName )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->addPatch(_par_patchName);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  FoamXServer::PatchToolServer::IPatchToolServer_stub::addPatch(_par_patchName);
}

#endif // MICO_CONF_NO_POA

void FoamXServer::PatchToolServer::IPatchToolServer_stub::deletePatch( const char* _par_patchName )
{
  CORBA::StaticAny _sa_patchName( CORBA::_stc_string, &_par_patchName );
  CORBA::StaticRequest __req( this, "deletePatch" );
  __req.add_in_arg( &_sa_patchName );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::deletePatch( const char* _par_patchName )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->deletePatch(_par_patchName);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  FoamXServer::PatchToolServer::IPatchToolServer_stub::deletePatch(_par_patchName);
}

#endif // MICO_CONF_NO_POA

void FoamXServer::PatchToolServer::IPatchToolServer_stub::changePatchType( const char* _par_patchName, const char* _par_patchType )
{
  CORBA::StaticAny _sa_patchName( CORBA::_stc_string, &_par_patchName );
  CORBA::StaticAny _sa_patchType( CORBA::_stc_string, &_par_patchType );
  CORBA::StaticRequest __req( this, "changePatchType" );
  __req.add_in_arg( &_sa_patchName );
  __req.add_in_arg( &_sa_patchType );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    0);
}


#ifndef MICO_CONF_NO_POA

void
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::changePatchType( const char* _par_patchName, const char* _par_patchType )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->changePatchType(_par_patchName, _par_patchType);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  FoamXServer::PatchToolServer::IPatchToolServer_stub::changePatchType(_par_patchName, _par_patchType);
}

#endif // MICO_CONF_NO_POA

void FoamXServer::PatchToolServer::IPatchToolServer_stub::changeFaces( const FoamXServer::LongList& _par_patchIDs, FoamXServer::LongList_out _par_oldToNew )
{
  CORBA::StaticAny _sa_patchIDs( CORBA::_stcseq_long, &_par_patchIDs );
  CORBA::StaticAny _sa_oldToNew( CORBA::_stcseq_long );
  CORBA::StaticRequest __req( this, "changeFaces" );
  __req.add_in_arg( &_sa_patchIDs );
  __req.add_out_arg( &_sa_oldToNew );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    0);
  _par_oldToNew = (FoamXServer::LongList*) _sa_oldToNew._retn();
}


#ifndef MICO_CONF_NO_POA

void
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::changeFaces( const FoamXServer::LongList& _par_patchIDs, FoamXServer::LongList_out _par_oldToNew )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->changeFaces(_par_patchIDs, _par_oldToNew);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  FoamXServer::PatchToolServer::IPatchToolServer_stub::changeFaces(_par_patchIDs, _par_oldToNew);
}

#endif // MICO_CONF_NO_POA

CORBA::Long FoamXServer::PatchToolServer::IPatchToolServer_stub::getNTris( CORBA::Long _par_startFaceI, CORBA::Long _par_nFaces, FoamXServer::LongList_out _par_nTris )
{
  CORBA::StaticAny _sa_startFaceI( CORBA::_stc_long, &_par_startFaceI );
  CORBA::StaticAny _sa_nFaces( CORBA::_stc_long, &_par_nFaces );
  CORBA::StaticAny _sa_nTris( CORBA::_stcseq_long );
  CORBA::Long _res;
  CORBA::StaticAny __res( CORBA::_stc_long, &_res );

  CORBA::StaticRequest __req( this, "getNTris" );
  __req.add_in_arg( &_sa_startFaceI );
  __req.add_in_arg( &_sa_nFaces );
  __req.add_out_arg( &_sa_nTris );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    0);
  _par_nTris = (FoamXServer::LongList*) _sa_nTris._retn();
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Long
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::getNTris( CORBA::Long _par_startFaceI, CORBA::Long _par_nFaces, FoamXServer::LongList_out _par_nTris )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      CORBA::Long __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->getNTris(_par_startFaceI, _par_nFaces, _par_nTris);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return FoamXServer::PatchToolServer::IPatchToolServer_stub::getNTris(_par_startFaceI, _par_nFaces, _par_nTris);
}

#endif // MICO_CONF_NO_POA

void FoamXServer::PatchToolServer::IPatchToolServer_stub::triangulate( CORBA::Long _par_startFaceI, CORBA::Long _par_nFaces, CORBA::Long _par_totalNTris, FoamXServer::LongList_out _par_triVerts )
{
  CORBA::StaticAny _sa_startFaceI( CORBA::_stc_long, &_par_startFaceI );
  CORBA::StaticAny _sa_nFaces( CORBA::_stc_long, &_par_nFaces );
  CORBA::StaticAny _sa_totalNTris( CORBA::_stc_long, &_par_totalNTris );
  CORBA::StaticAny _sa_triVerts( CORBA::_stcseq_long );
  CORBA::StaticRequest __req( this, "triangulate" );
  __req.add_in_arg( &_sa_startFaceI );
  __req.add_in_arg( &_sa_nFaces );
  __req.add_in_arg( &_sa_totalNTris );
  __req.add_out_arg( &_sa_triVerts );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    0);
  _par_triVerts = (FoamXServer::LongList*) _sa_triVerts._retn();
}


#ifndef MICO_CONF_NO_POA

void
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::triangulate( CORBA::Long _par_startFaceI, CORBA::Long _par_nFaces, CORBA::Long _par_totalNTris, FoamXServer::LongList_out _par_triVerts )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->triangulate(_par_startFaceI, _par_nFaces, _par_totalNTris, _par_triVerts);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  FoamXServer::PatchToolServer::IPatchToolServer_stub::triangulate(_par_startFaceI, _par_nFaces, _par_totalNTris, _par_triVerts);
}

#endif // MICO_CONF_NO_POA

CORBA::Long FoamXServer::PatchToolServer::IPatchToolServer_stub::getNPoints( CORBA::Long _par_startFaceI, CORBA::Long _par_nFaces )
{
  CORBA::StaticAny _sa_startFaceI( CORBA::_stc_long, &_par_startFaceI );
  CORBA::StaticAny _sa_nFaces( CORBA::_stc_long, &_par_nFaces );
  CORBA::Long _res;
  CORBA::StaticAny __res( CORBA::_stc_long, &_res );

  CORBA::StaticRequest __req( this, "getNPoints" );
  __req.add_in_arg( &_sa_startFaceI );
  __req.add_in_arg( &_sa_nFaces );
  __req.set_result( &__res );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    0);
  return _res;
}


#ifndef MICO_CONF_NO_POA

CORBA::Long
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::getNPoints( CORBA::Long _par_startFaceI, CORBA::Long _par_nFaces )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      CORBA::Long __res;

      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        __res = _myserv->getNPoints(_par_startFaceI, _par_nFaces);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return __res;
    }
    _postinvoke ();
  }

  return FoamXServer::PatchToolServer::IPatchToolServer_stub::getNPoints(_par_startFaceI, _par_nFaces);
}

#endif // MICO_CONF_NO_POA

void FoamXServer::PatchToolServer::IPatchToolServer_stub::triangulateLocal( CORBA::Long _par_startFaceI, CORBA::Long _par_nFaces, CORBA::Long _par_totalNTris, FoamXServer::LongList_out _par_triVerts, FoamXServer::LongList_out _par_localToGlobal )
{
  CORBA::StaticAny _sa_startFaceI( CORBA::_stc_long, &_par_startFaceI );
  CORBA::StaticAny _sa_nFaces( CORBA::_stc_long, &_par_nFaces );
  CORBA::StaticAny _sa_totalNTris( CORBA::_stc_long, &_par_totalNTris );
  CORBA::StaticAny _sa_triVerts( CORBA::_stcseq_long );
  CORBA::StaticAny _sa_localToGlobal( CORBA::_stcseq_long );
  CORBA::StaticRequest __req( this, "triangulateLocal" );
  __req.add_in_arg( &_sa_startFaceI );
  __req.add_in_arg( &_sa_nFaces );
  __req.add_in_arg( &_sa_totalNTris );
  __req.add_out_arg( &_sa_triVerts );
  __req.add_out_arg( &_sa_localToGlobal );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    0);
  _par_triVerts = (FoamXServer::LongList*) _sa_triVerts._retn();
  _par_localToGlobal = (FoamXServer::LongList*) _sa_localToGlobal._retn();
}


#ifndef MICO_CONF_NO_POA

void
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::triangulateLocal( CORBA::Long _par_startFaceI, CORBA::Long _par_nFaces, CORBA::Long _par_totalNTris, FoamXServer::LongList_out _par_triVerts, FoamXServer::LongList_out _par_localToGlobal )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->triangulateLocal(_par_startFaceI, _par_nFaces, _par_totalNTris, _par_triVerts, _par_localToGlobal);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  FoamXServer::PatchToolServer::IPatchToolServer_stub::triangulateLocal(_par_startFaceI, _par_nFaces, _par_totalNTris, _par_triVerts, _par_localToGlobal);
}

#endif // MICO_CONF_NO_POA

void FoamXServer::PatchToolServer::IPatchToolServer_stub::markFaces( const FoamXServer::LongList& _par_protectedEdges, CORBA::Long _par_faceI, FoamXServer::BoolList_out _par_visited )
{
  CORBA::StaticAny _sa_protectedEdges( CORBA::_stcseq_long, &_par_protectedEdges );
  CORBA::StaticAny _sa_faceI( CORBA::_stc_long, &_par_faceI );
  CORBA::StaticAny _sa_visited( CORBA::_stcseq_boolean );
  CORBA::StaticRequest __req( this, "markFaces" );
  __req.add_in_arg( &_sa_protectedEdges );
  __req.add_in_arg( &_sa_faceI );
  __req.add_out_arg( &_sa_visited );

  __req.invoke();

  mico_sii_throw( &__req, 
    _marshaller_FoamXServer_FoamXError, "IDL:FoamXServer/FoamXError:1.0",
    0);
  _par_visited = (FoamXServer::BoolList*) _sa_visited._retn();
}


#ifndef MICO_CONF_NO_POA

void
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::markFaces( const FoamXServer::LongList& _par_protectedEdges, CORBA::Long _par_faceI, FoamXServer::BoolList_out _par_visited )
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      #ifdef HAVE_EXCEPTIONS
      try {
      #endif
        _myserv->markFaces(_par_protectedEdges, _par_faceI, _par_visited);
      #ifdef HAVE_EXCEPTIONS
      }
      catch (...) {
        _myserv->_remove_ref();
        _postinvoke();
        throw;
      }
      #endif

      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  FoamXServer::PatchToolServer::IPatchToolServer_stub::markFaces(_par_protectedEdges, _par_faceI, _par_visited);
}

#endif // MICO_CONF_NO_POA

void FoamXServer::PatchToolServer::IPatchToolServer_stub::close()
{
  CORBA::StaticRequest __req( this, "close" );

  __req.oneway();

  mico_sii_throw( &__req, 
    0);
}


#ifndef MICO_CONF_NO_POA

void
FoamXServer::PatchToolServer::IPatchToolServer_stub_clp::close()
{
  PortableServer::Servant _serv = _preinvoke ();
  if (_serv) {
    POA_FoamXServer::PatchToolServer::IPatchToolServer * _myserv = POA_FoamXServer::PatchToolServer::IPatchToolServer::_narrow (_serv);
    if (_myserv) {
      _myserv->close();
      _myserv->_remove_ref();
      _postinvoke ();
      return;
    }
    _postinvoke ();
  }

  FoamXServer::PatchToolServer::IPatchToolServer_stub::close();
}

#endif // MICO_CONF_NO_POA

class _Marshaller__seq__seq_long : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< FoamXServer::LongList,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq__seq_long();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq__seq_long::~_Marshaller__seq__seq_long()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq__seq_long::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq__seq_long::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq__seq_long::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq__seq_long::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !CORBA::_stcseq_long->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq__seq_long::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    CORBA::_stcseq_long->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq__seq_long::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "01000000130000005c00000001000000150000004c000000010000001d00"
    "000049444c3a466f616d585365727665722f4c6f6e674c6973743a312e30"
    "00000000090000004c6f6e674c69737400000000130000000c0000000100"
    "0000030000000000000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq__seq_long::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq__seq_long;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< FoamXServer::LongList,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq__seq_long, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< FoamXServer::LongList,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< FoamXServer::LongList,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq__seq_long, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< FoamXServer::LongList,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq__seq_long, (void *&)_s);
}


class _Marshaller__a3_float : public ::CORBA::StaticTypeInfo {
    typedef CORBA::Float _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__a3_float();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__a3_float::~_Marshaller__a3_float()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__a3_float::create() const
{
  return (StaticValueType) new _MICO_T[ 3 ];
}

void _Marshaller__a3_float::assign( StaticValueType d, const StaticValueType s ) const
{
  for( int i = 0; i < 3; i++ )
    ((CORBA::Float *) d)[ i ] = ((CORBA::Float *) s)[ i ];
}

void _Marshaller__a3_float::free( StaticValueType v ) const
{
  delete[] (_MICO_T *) v;
}

::CORBA::Boolean _Marshaller__a3_float::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  if( !dc.arr_begin() )
    return FALSE;
  if (!dc.get_floats (&((_MICO_T *)v)[0], 3))
    return FALSE;
  return dc.arr_end();
}

void _Marshaller__a3_float::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ec.arr_begin();
  ec.put_floats (&((_MICO_T *)v)[0], 3);
  ec.arr_end();
}

::CORBA::TypeCode_ptr _Marshaller__a3_float::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "01000000140000000c000000010000000600000003000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__a3_float::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__a3_float;

class _Marshaller__seq_FoamXServer_FoamXAny : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< FoamXServer::FoamXAny,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_FoamXServer_FoamXAny();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_FoamXServer_FoamXAny::~_Marshaller__seq_FoamXServer_FoamXAny()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_FoamXServer_FoamXAny::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_FoamXServer_FoamXAny::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_FoamXServer_FoamXAny::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_FoamXServer_FoamXAny::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_FoamXServer_FoamXAny->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_FoamXServer_FoamXAny::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_FoamXServer_FoamXAny->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_FoamXServer_FoamXAny::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000038020000010000000f00000028020000010000001d00"
    "000049444c3a466f616d585365727665722f466f616d58416e793a312e30"
    "0000000009000000466f616d58416e790000000002000000050000007479"
    "70650000000011000000c7010000010000001e00000049444c3a466f616d"
    "585365727665722f466f616d58547970653a312e300000000a000000466f"
    "616d5854797065000000150000000f000000547970655f556e646566696e"
    "656400000d000000547970655f426f6f6c65616e000000000b0000005479"
    "70655f4c6162656c00000c000000547970655f5363616c6172000a000000"
    "547970655f436861720000000a000000547970655f576f72640000000c00"
    "0000547970655f537472696e67000d000000547970655f526f6f74446972"
    "0000000011000000547970655f526f6f74416e6443617365000000000e00"
    "0000547970655f436173654e616d650000000e000000547970655f486f73"
    "744e616d650000000a000000547970655f46696c650000000f0000005479"
    "70655f4469726563746f727900000a000000547970655f54696d65000000"
    "12000000547970655f44696d656e73696f6e536574000000110000005479"
    "70655f566563746f725370616365000000000a000000547970655f4c6973"
    "7400000010000000547970655f44696374696f6e617279000f0000005479"
    "70655f53656c656374696f6e00000e000000547970655f436f6d706f756e"
    "640000000b000000547970655f4669656c6400000600000076616c756500"
    "00000b00000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_FoamXServer_FoamXAny::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_FoamXServer_FoamXAny;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< FoamXServer::FoamXAny,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_FoamXServer_FoamXAny, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< FoamXServer::FoamXAny,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< FoamXServer::FoamXAny,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_FoamXServer_FoamXAny, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< FoamXServer::FoamXAny,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_FoamXServer_FoamXAny, (void *&)_s);
}


class _Marshaller__seq_FoamXServer_StringPair : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< FoamXServer::StringPair,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_FoamXServer_StringPair();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_FoamXServer_StringPair::~_Marshaller__seq_FoamXServer_StringPair()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_FoamXServer_StringPair::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_FoamXServer_StringPair::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_FoamXServer_StringPair::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_FoamXServer_StringPair::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_FoamXServer_StringPair->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_FoamXServer_StringPair::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_FoamXServer_StringPair->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_FoamXServer_StringPair::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000074000000010000000f00000064000000010000001f00"
    "000049444c3a466f616d585365727665722f537472696e67506169723a31"
    "2e3000000b000000537472696e6750616972000002000000050000006e61"
    "6d650000000012000000000000000600000076616c756500000012000000"
    "0000000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_FoamXServer_StringPair::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_FoamXServer_StringPair;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< FoamXServer::StringPair,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_FoamXServer_StringPair, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< FoamXServer::StringPair,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< FoamXServer::StringPair,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_FoamXServer_StringPair, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< FoamXServer::StringPair,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_FoamXServer_StringPair, (void *&)_s);
}


class _Marshaller__seq_FoamXServer_HostDescriptor : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< FoamXServer::HostDescriptor,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_FoamXServer_HostDescriptor();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_FoamXServer_HostDescriptor::~_Marshaller__seq_FoamXServer_HostDescriptor()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_FoamXServer_HostDescriptor::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_FoamXServer_HostDescriptor::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_FoamXServer_HostDescriptor::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_FoamXServer_HostDescriptor::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_FoamXServer_HostDescriptor->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_FoamXServer_HostDescriptor::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_FoamXServer_HostDescriptor->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_FoamXServer_HostDescriptor::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000078000000010000000f00000068000000010000002300"
    "000049444c3a466f616d585365727665722f486f73744465736372697074"
    "6f723a312e3000000f000000486f737444657363726970746f7200000200"
    "0000050000006e616d6500000000120000000000000006000000616c6976"
    "650000000800000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_FoamXServer_HostDescriptor::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_FoamXServer_HostDescriptor;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< FoamXServer::HostDescriptor,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_FoamXServer_HostDescriptor, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< FoamXServer::HostDescriptor,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< FoamXServer::HostDescriptor,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_FoamXServer_HostDescriptor, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< FoamXServer::HostDescriptor,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_FoamXServer_HostDescriptor, (void *&)_s);
}


class _Marshaller__seq_FoamXServer_ApplicationClassDescriptor : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< FoamXServer::ApplicationClassDescriptor,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_FoamXServer_ApplicationClassDescriptor();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_FoamXServer_ApplicationClassDescriptor::~_Marshaller__seq_FoamXServer_ApplicationClassDescriptor()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_FoamXServer_ApplicationClassDescriptor::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_FoamXServer_ApplicationClassDescriptor::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_FoamXServer_ApplicationClassDescriptor::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_FoamXServer_ApplicationClassDescriptor::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_FoamXServer_ApplicationClassDescriptor->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_FoamXServer_ApplicationClassDescriptor::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_FoamXServer_ApplicationClassDescriptor->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_FoamXServer_ApplicationClassDescriptor::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "0100000013000000ac000000010000000f0000009c000000010000002f00"
    "000049444c3a466f616d585365727665722f4170706c69636174696f6e43"
    "6c61737344657363726970746f723a312e3000001b0000004170706c6963"
    "6174696f6e436c61737344657363726970746f7200000300000005000000"
    "6e616d650000000012000000000000000900000063617465676f72790000"
    "000012000000000000000c00000073797374656d436c6173730008000000"
    "00000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_FoamXServer_ApplicationClassDescriptor::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_FoamXServer_ApplicationClassDescriptor;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< FoamXServer::ApplicationClassDescriptor,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_FoamXServer_ApplicationClassDescriptor, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< FoamXServer::ApplicationClassDescriptor,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< FoamXServer::ApplicationClassDescriptor,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_FoamXServer_ApplicationClassDescriptor, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< FoamXServer::ApplicationClassDescriptor,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_FoamXServer_ApplicationClassDescriptor, (void *&)_s);
}


class _Marshaller__seq_FoamXServer_CaseDescriptor : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< FoamXServer::CaseDescriptor,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_FoamXServer_CaseDescriptor();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_FoamXServer_CaseDescriptor::~_Marshaller__seq_FoamXServer_CaseDescriptor()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_FoamXServer_CaseDescriptor::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_FoamXServer_CaseDescriptor::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_FoamXServer_CaseDescriptor::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_FoamXServer_CaseDescriptor::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_FoamXServer_CaseDescriptor->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_FoamXServer_CaseDescriptor::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_FoamXServer_CaseDescriptor->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_FoamXServer_CaseDescriptor::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "0100000013000000f0000000010000000f000000e0000000010000002300"
    "000049444c3a466f616d585365727665722f436173654465736372697074"
    "6f723a312e3000000f0000004361736544657363726970746f7200000800"
    "000008000000726f6f744469720012000000000000000b00000072617752"
    "6f6f744469720000120000000000000009000000636173654e616d650000"
    "0000120000000000000009000000617070436c6173730000000012000000"
    "00000000070000006e50726f6373000003000000080000006d616e616765"
    "640008000000070000006c6f636b6564000008000000060000006572726f"
    "720000000800000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_FoamXServer_CaseDescriptor::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_FoamXServer_CaseDescriptor;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< FoamXServer::CaseDescriptor,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_FoamXServer_CaseDescriptor, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< FoamXServer::CaseDescriptor,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< FoamXServer::CaseDescriptor,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_FoamXServer_CaseDescriptor, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< FoamXServer::CaseDescriptor,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_FoamXServer_CaseDescriptor, (void *&)_s);
}


class _Marshaller__seq_FoamXServer_JobID : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< FoamXServer::JobID,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_FoamXServer_JobID();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_FoamXServer_JobID::~_Marshaller__seq_FoamXServer_JobID()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_FoamXServer_JobID::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_FoamXServer_JobID::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_FoamXServer_JobID::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_FoamXServer_JobID::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_FoamXServer_JobID->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_FoamXServer_JobID::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_FoamXServer_JobID->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_FoamXServer_JobID::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "010000001300000070000000010000000f00000060000000010000001a00"
    "000049444c3a466f616d585365727665722f4a6f6249443a312e30000000"
    "060000004a6f6249440000000200000009000000686f73744e616d650000"
    "000012000000000000000a00000070726f63657373494400000003000000"
    "00000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_FoamXServer_JobID::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_FoamXServer_JobID;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< FoamXServer::JobID,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_FoamXServer_JobID, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< FoamXServer::JobID,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< FoamXServer::JobID,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_FoamXServer_JobID, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< FoamXServer::JobID,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_FoamXServer_JobID, (void *&)_s);
}


class _Marshaller__seq_FoamXServer_JobDescriptor : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< FoamXServer::JobDescriptor,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_FoamXServer_JobDescriptor();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_FoamXServer_JobDescriptor::~_Marshaller__seq_FoamXServer_JobDescriptor()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_FoamXServer_JobDescriptor::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_FoamXServer_JobDescriptor::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_FoamXServer_JobDescriptor::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_FoamXServer_JobDescriptor::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_FoamXServer_JobDescriptor->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_FoamXServer_JobDescriptor::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_FoamXServer_JobDescriptor->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_FoamXServer_JobDescriptor::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "0100000013000000ac030000010000000f0000009c030000010000002200"
    "000049444c3a466f616d585365727665722f4a6f6244657363726970746f"
    "723a312e300000000e0000004a6f6244657363726970746f720000001300"
    "0000060000006a6f6249440000000f00000060000000010000001a000000"
    "49444c3a466f616d585365727665722f4a6f6249443a312e300000000600"
    "00004a6f6249440000000200000009000000686f73744e616d6500000000"
    "12000000000000000a00000070726f636573734944000000030000000500"
    "000070706964000000000300000005000000706769640000000003000000"
    "0a00000073746172744461746500000012000000000000000a0000007374"
    "61727454696d65000000120000000000000009000000757365724e616d65"
    "0000000012000000000000000c000000666f616d56657273696f6e001200"
    "00000000000005000000636f646500000000120000000000000008000000"
    "6172674c6973740012000000000000000b00000063757272656e74446972"
    "0000120000000000000008000000726f6f74446972001200000000000000"
    "09000000636173654e616d65000000001200000000000000070000006e50"
    "726f637300000300000007000000736c61766573000015000000b0000000"
    "010000001e00000049444c3a466f616d585365727665722f4a6f6249444c"
    "6973743a312e300000000a0000004a6f6249444c69737400000013000000"
    "70000000010000000f00000060000000010000001a00000049444c3a466f"
    "616d585365727665722f4a6f6249443a312e30000000060000004a6f6249"
    "440000000200000009000000686f73744e616d6500000000120000000000"
    "00000a00000070726f63657373494400000003000000000000000e000000"
    "6e436f756e74656450726f6373000000030000000800000063707554696d"
    "65000700000008000000656e644461746500120000000000000008000000"
    "656e6454696d650012000000000000000700000073746174757300001100"
    "0000c0000000010000001e00000049444c3a466f616d585365727665722f"
    "4a6f625374617475733a312e300000000a0000004a6f6253746174757300"
    "0000070000000e0000004a4f425f554e444546494e45440000000e000000"
    "4a4f425f4c41554e4348494e470000000c0000004a4f425f52554e4e494e"
    "47000d0000004a4f425f53544f5050494e47000000000e0000004a4f425f"
    "53555350454e4445440000000d0000004a4f425f46494e49534845440000"
    "00000c0000004a4f425f41424f525445440000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_FoamXServer_JobDescriptor::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_FoamXServer_JobDescriptor;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< FoamXServer::JobDescriptor,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_FoamXServer_JobDescriptor, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< FoamXServer::JobDescriptor,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< FoamXServer::JobDescriptor,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_FoamXServer_JobDescriptor, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< FoamXServer::JobDescriptor,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_FoamXServer_JobDescriptor, (void *&)_s);
}


class _Marshaller__seq_FoamXServer_DimensionSet : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< FoamXServer::DimensionSet,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_FoamXServer_DimensionSet();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_FoamXServer_DimensionSet::~_Marshaller__seq_FoamXServer_DimensionSet()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_FoamXServer_DimensionSet::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_FoamXServer_DimensionSet::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_FoamXServer_DimensionSet::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_FoamXServer_DimensionSet::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_FoamXServer_DimensionSet->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_FoamXServer_DimensionSet::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_FoamXServer_DimensionSet->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_FoamXServer_DimensionSet::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "0100000013000000d4000000010000000f000000c4000000010000002100"
    "000049444c3a466f616d585365727665722f44696d656e73696f6e536574"
    "3a312e30000000000d00000044696d656e73696f6e536574000000000700"
    "0000050000006d6173730000000007000000070000006c656e6774680000"
    "070000000500000074696d6500000000070000000c00000074656d706572"
    "61747572650007000000060000006d6f6c65730000000700000008000000"
    "63757272656e740007000000120000006c756d696e6f7573496e74656e73"
    "6974790000000700000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_FoamXServer_DimensionSet::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_FoamXServer_DimensionSet;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< FoamXServer::DimensionSet,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_FoamXServer_DimensionSet, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< FoamXServer::DimensionSet,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< FoamXServer::DimensionSet,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_FoamXServer_DimensionSet, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< FoamXServer::DimensionSet,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_FoamXServer_DimensionSet, (void *&)_s);
}


class _Marshaller__seq_FoamXServer_SurfacePatch : public ::CORBA::StaticTypeInfo {
    typedef SequenceTmpl< FoamXServer::SurfacePatch,MICO_TID_DEF> _MICO_T;
    static ::CORBA::TypeCode_ptr _tc;
  public:
    ~_Marshaller__seq_FoamXServer_SurfacePatch();
    StaticValueType create () const;
    void assign (StaticValueType dst, const StaticValueType src) const;
    void free (StaticValueType) const;
    ::CORBA::Boolean demarshal (::CORBA::DataDecoder&, StaticValueType) const;
    void marshal (::CORBA::DataEncoder &, StaticValueType) const;
    ::CORBA::TypeCode_ptr typecode ();
};


_Marshaller__seq_FoamXServer_SurfacePatch::~_Marshaller__seq_FoamXServer_SurfacePatch()
{
  if (_tc)
    delete _tc;
}

::CORBA::StaticValueType _Marshaller__seq_FoamXServer_SurfacePatch::create() const
{
  return (StaticValueType) new _MICO_T;
}

void _Marshaller__seq_FoamXServer_SurfacePatch::assign( StaticValueType d, const StaticValueType s ) const
{
  *(_MICO_T*) d = *(_MICO_T*) s;
}

void _Marshaller__seq_FoamXServer_SurfacePatch::free( StaticValueType v ) const
{
  delete (_MICO_T*) v;
}

::CORBA::Boolean _Marshaller__seq_FoamXServer_SurfacePatch::demarshal( ::CORBA::DataDecoder &dc, StaticValueType v ) const
{
  ::CORBA::ULong len;
  if( !dc.seq_begin( len ) )
    return FALSE;
  ((_MICO_T *) v)->length( len );
  for( ::CORBA::ULong i = 0; i < len; i++ ) {
    if( !_marshaller_FoamXServer_SurfacePatch->demarshal( dc, &(*(_MICO_T*)v)[i] ) )
      return FALSE;
  }
  return dc.seq_end();
}

void _Marshaller__seq_FoamXServer_SurfacePatch::marshal( ::CORBA::DataEncoder &ec, StaticValueType v ) const
{
  ::CORBA::ULong len = ((_MICO_T *) v)->length();
  ec.seq_begin( len );
  for( ::CORBA::ULong i = 0; i < len; i++ )
    _marshaller_FoamXServer_SurfacePatch->marshal( ec, &(*(_MICO_T*)v)[i] );
  ec.seq_end();
}

::CORBA::TypeCode_ptr _Marshaller__seq_FoamXServer_SurfacePatch::typecode()
{
  if (!_tc)
    _tc = (new ::CORBA::TypeCode (
    "0100000013000000a4000000010000000f00000094000000010000002100"
    "000049444c3a466f616d585365727665722f537572666163655061746368"
    "3a312e30000000000d000000537572666163655061746368000000000400"
    "0000050000006e616d650000000012000000000000000e00000067656f6d"
    "65747269635479706500000012000000000000000500000073697a650000"
    "0000030000000600000073746172740000000300000000000000"))->mk_constant();
  return _tc;
}

::CORBA::TypeCode_ptr _Marshaller__seq_FoamXServer_SurfacePatch::_tc = 0;
::CORBA::StaticTypeInfo *_marshaller__seq_FoamXServer_SurfacePatch;

void operator<<=( CORBA::Any &_a, const SequenceTmpl< FoamXServer::SurfacePatch,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_FoamXServer_SurfacePatch, &_s);
  _a.from_static_any (_sa);
}

void operator<<=( CORBA::Any &_a, SequenceTmpl< FoamXServer::SurfacePatch,MICO_TID_DEF> *_s )
{
  _a <<= *_s;
  delete _s;
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, SequenceTmpl< FoamXServer::SurfacePatch,MICO_TID_DEF> &_s )
{
  CORBA::StaticAny _sa (_marshaller__seq_FoamXServer_SurfacePatch, &_s);
  return _a.to_static_any (_sa);
}

CORBA::Boolean operator>>=( const CORBA::Any &_a, const SequenceTmpl< FoamXServer::SurfacePatch,MICO_TID_DEF> *&_s )
{
  return _a.to_static_any (_marshaller__seq_FoamXServer_SurfacePatch, (void *&)_s);
}


struct __tc_init_PATCHTOOL {
  __tc_init_PATCHTOOL()
  {
    FoamXServer::_tc_FoamXType = 
    "0100000011000000c7010000010000001e00000049444c3a466f616d5853"
    "65727665722f466f616d58547970653a312e300000000a000000466f616d"
    "5854797065000000150000000f000000547970655f556e646566696e6564"
    "00000d000000547970655f426f6f6c65616e000000000b00000054797065"
    "5f4c6162656c00000c000000547970655f5363616c6172000a0000005479"
    "70655f436861720000000a000000547970655f576f72640000000c000000"
    "547970655f537472696e67000d000000547970655f526f6f744469720000"
    "000011000000547970655f526f6f74416e6443617365000000000e000000"
    "547970655f436173654e616d650000000e000000547970655f486f73744e"
    "616d650000000a000000547970655f46696c650000000f00000054797065"
    "5f4469726563746f727900000a000000547970655f54696d650000001200"
    "0000547970655f44696d656e73696f6e5365740000001100000054797065"
    "5f566563746f725370616365000000000a000000547970655f4c69737400"
    "000010000000547970655f44696374696f6e617279000f00000054797065"
    "5f53656c656374696f6e00000e000000547970655f436f6d706f756e6400"
    "00000b000000547970655f4669656c6400";
    _marshaller_FoamXServer_FoamXType = new _Marshaller_FoamXServer_FoamXType;
    FoamXServer::_tc_ErrorCode = 
    "0100000011000000dd000000010000001e00000049444c3a466f616d5853"
    "65727665722f4572726f72436f64653a312e300000000a0000004572726f"
    "72436f64650000000900000005000000535f4f4b0000000007000000455f"
    "4641494c000007000000455f464f414d00000e000000455f494e56414c49"
    "445f4152470000000e000000455f494e56414c49445f5054520000000e00"
    "0000455f494e56414c49445f52454600000016000000455f494e4445585f"
    "4f55545f4f465f424f554e44530000000f000000455f554e4b4e4f574e5f"
    "4e414d4500000d000000455f554e455850454354454400";
    _marshaller_FoamXServer_ErrorCode = new _Marshaller_FoamXServer_ErrorCode;
    FoamXServer::_tc_JobStatus = 
    "0100000011000000c0000000010000001e00000049444c3a466f616d5853"
    "65727665722f4a6f625374617475733a312e300000000a0000004a6f6253"
    "7461747573000000070000000e0000004a4f425f554e444546494e454400"
    "00000e0000004a4f425f4c41554e4348494e470000000c0000004a4f425f"
    "52554e4e494e47000d0000004a4f425f53544f5050494e47000000000e00"
    "00004a4f425f53555350454e4445440000000d0000004a4f425f46494e49"
    "53484544000000000c0000004a4f425f41424f5254454400";
    _marshaller_FoamXServer_JobStatus = new _Marshaller_FoamXServer_JobStatus;
    FoamXServer::_tc_MessageType = 
    "01000000110000006c000000010000002000000049444c3a466f616d5853"
    "65727665722f4d657373616765547970653a312e30000c0000004d657373"
    "6167655479706500030000000d0000004d5f444941474e4f535449430000"
    "00000a0000004d5f5741524e494e47000000080000004d5f4552524f5200"
    ;
    _marshaller_FoamXServer_MessageType = new _Marshaller_FoamXServer_MessageType;
    FoamXServer::_tc_StringList = 
    "010000001500000050000000010000001f00000049444c3a466f616d5853"
    "65727665722f537472696e674c6973743a312e3000000b00000053747269"
    "6e674c697374000013000000100000000100000012000000000000000000"
    "0000";
    FoamXServer::_tc_DoubleList = 
    "01000000150000004c000000010000001f00000049444c3a466f616d5853"
    "65727665722f446f75626c654c6973743a312e3000000b000000446f7562"
    "6c654c6973740000130000000c000000010000000700000000000000";
    FoamXServer::_tc_FloatList = 
    "01000000150000004c000000010000001e00000049444c3a466f616d5853"
    "65727665722f466c6f61744c6973743a312e300000000a000000466c6f61"
    "744c697374000000130000000c000000010000000600000000000000";
    FoamXServer::_tc_BoolList = 
    "01000000150000004c000000010000001d00000049444c3a466f616d5853"
    "65727665722f426f6f6c4c6973743a312e300000000009000000426f6f6c"
    "4c69737400000000130000000c000000010000000800000000000000";
    FoamXServer::_tc_LongList = 
    "01000000150000004c000000010000001d00000049444c3a466f616d5853"
    "65727665722f4c6f6e674c6973743a312e3000000000090000004c6f6e67"
    "4c69737400000000130000000c000000010000000300000000000000";
    FoamXServer::_tc_LongLongList = 
    "0100000015000000a4000000010000002100000049444c3a466f616d5853"
    "65727665722f4c6f6e674c6f6e674c6973743a312e30000000000d000000"
    "4c6f6e674c6f6e674c69737400000000130000005c000000010000001500"
    "00004c000000010000001d00000049444c3a466f616d585365727665722f"
    "4c6f6e674c6973743a312e3000000000090000004c6f6e674c6973740000"
    "0000130000000c00000001000000030000000000000000000000";
    FoamXServer::_tc_Point3 = 
    "010000001500000044000000010000001b00000049444c3a466f616d5853"
    "65727665722f506f696e74333a312e30000007000000506f696e74330000"
    "140000000c000000010000000600000003000000";
    FoamXServer::_tc_FoamXAny = 
    "010000000f00000028020000010000001d00000049444c3a466f616d5853"
    "65727665722f466f616d58416e793a312e300000000009000000466f616d"
    "58416e79000000000200000005000000747970650000000011000000c701"
    "0000010000001e00000049444c3a466f616d585365727665722f466f616d"
    "58547970653a312e300000000a000000466f616d58547970650000001500"
    "00000f000000547970655f556e646566696e656400000d00000054797065"
    "5f426f6f6c65616e000000000b000000547970655f4c6162656c00000c00"
    "0000547970655f5363616c6172000a000000547970655f43686172000000"
    "0a000000547970655f576f72640000000c000000547970655f537472696e"
    "67000d000000547970655f526f6f74446972000000001100000054797065"
    "5f526f6f74416e6443617365000000000e000000547970655f436173654e"
    "616d650000000e000000547970655f486f73744e616d650000000a000000"
    "547970655f46696c650000000f000000547970655f4469726563746f7279"
    "00000a000000547970655f54696d6500000012000000547970655f44696d"
    "656e73696f6e53657400000011000000547970655f566563746f72537061"
    "6365000000000a000000547970655f4c6973740000001000000054797065"
    "5f44696374696f6e617279000f000000547970655f53656c656374696f6e"
    "00000e000000547970655f436f6d706f756e640000000b00000054797065"
    "5f4669656c6400000600000076616c75650000000b000000";
    _marshaller_FoamXServer_FoamXAny = new _Marshaller_FoamXServer_FoamXAny;
    FoamXServer::_tc_FoamXAnyList = 
    "010000001500000080020000010000002100000049444c3a466f616d5853"
    "65727665722f466f616d58416e794c6973743a312e30000000000d000000"
    "466f616d58416e794c697374000000001300000038020000010000000f00"
    "000028020000010000001d00000049444c3a466f616d585365727665722f"
    "466f616d58416e793a312e300000000009000000466f616d58416e790000"
    "00000200000005000000747970650000000011000000c701000001000000"
    "1e00000049444c3a466f616d585365727665722f466f616d58547970653a"
    "312e300000000a000000466f616d5854797065000000150000000f000000"
    "547970655f556e646566696e656400000d000000547970655f426f6f6c65"
    "616e000000000b000000547970655f4c6162656c00000c00000054797065"
    "5f5363616c6172000a000000547970655f436861720000000a0000005479"
    "70655f576f72640000000c000000547970655f537472696e67000d000000"
    "547970655f526f6f744469720000000011000000547970655f526f6f7441"
    "6e6443617365000000000e000000547970655f436173654e616d65000000"
    "0e000000547970655f486f73744e616d650000000a000000547970655f46"
    "696c650000000f000000547970655f4469726563746f727900000a000000"
    "547970655f54696d6500000012000000547970655f44696d656e73696f6e"
    "53657400000011000000547970655f566563746f72537061636500000000"
    "0a000000547970655f4c69737400000010000000547970655f4469637469"
    "6f6e617279000f000000547970655f53656c656374696f6e00000e000000"
    "547970655f436f6d706f756e640000000b000000547970655f4669656c64"
    "00000600000076616c75650000000b00000000000000";
    FoamXServer::_tc_StringPair = 
    "010000000f00000064000000010000001f00000049444c3a466f616d5853"
    "65727665722f537472696e67506169723a312e3000000b00000053747269"
    "6e6750616972000002000000050000006e616d6500000000120000000000"
    "00000600000076616c75650000001200000000000000";
    _marshaller_FoamXServer_StringPair = new _Marshaller_FoamXServer_StringPair;
    FoamXServer::_tc_StringPairList = 
    "0100000015000000bc000000010000002300000049444c3a466f616d5853"
    "65727665722f537472696e67506169724c6973743a312e3000000f000000"
    "537472696e67506169724c69737400001300000074000000010000000f00"
    "000064000000010000001f00000049444c3a466f616d585365727665722f"
    "537472696e67506169723a312e3000000b000000537472696e6750616972"
    "000002000000050000006e616d6500000000120000000000000006000000"
    "76616c7565000000120000000000000000000000";
    FoamXServer::_tc_HostDescriptor = 
    "010000000f00000068000000010000002300000049444c3a466f616d5853"
    "65727665722f486f737444657363726970746f723a312e3000000f000000"
    "486f737444657363726970746f72000002000000050000006e616d650000"
    "0000120000000000000006000000616c69766500000008000000";
    _marshaller_FoamXServer_HostDescriptor = new _Marshaller_FoamXServer_HostDescriptor;
    FoamXServer::_tc_HostDescriptorList = 
    "0100000015000000c8000000010000002700000049444c3a466f616d5853"
    "65727665722f486f737444657363726970746f724c6973743a312e300000"
    "13000000486f737444657363726970746f724c6973740000130000007800"
    "0000010000000f00000068000000010000002300000049444c3a466f616d"
    "585365727665722f486f737444657363726970746f723a312e3000000f00"
    "0000486f737444657363726970746f72000002000000050000006e616d65"
    "00000000120000000000000006000000616c697665000000080000000000"
    "0000";
    FoamXServer::_tc_ApplicationClassDescriptor = 
    "010000000f0000009c000000010000002f00000049444c3a466f616d5853"
    "65727665722f4170706c69636174696f6e436c6173734465736372697074"
    "6f723a312e3000001b0000004170706c69636174696f6e436c6173734465"
    "7363726970746f72000003000000050000006e616d650000000012000000"
    "000000000900000063617465676f72790000000012000000000000000c00"
    "000073797374656d436c6173730008000000";
    _marshaller_FoamXServer_ApplicationClassDescriptor = new _Marshaller_FoamXServer_ApplicationClassDescriptor;
    FoamXServer::_tc_ApplicationClassDescriptorList = 
    "010000001500000014010000010000003300000049444c3a466f616d5853"
    "65727665722f4170706c69636174696f6e436c6173734465736372697074"
    "6f724c6973743a312e3000001f0000004170706c69636174696f6e436c61"
    "737344657363726970746f724c697374000013000000ac00000001000000"
    "0f0000009c000000010000002f00000049444c3a466f616d585365727665"
    "722f4170706c69636174696f6e436c61737344657363726970746f723a31"
    "2e3000001b0000004170706c69636174696f6e436c617373446573637269"
    "70746f72000003000000050000006e616d65000000001200000000000000"
    "0900000063617465676f72790000000012000000000000000c0000007379"
    "7374656d436c617373000800000000000000";
    FoamXServer::_tc_CaseDescriptor = 
    "010000000f000000e0000000010000002300000049444c3a466f616d5853"
    "65727665722f4361736544657363726970746f723a312e3000000f000000"
    "4361736544657363726970746f7200000800000008000000726f6f744469"
    "720012000000000000000b000000726177526f6f74446972000012000000"
    "0000000009000000636173654e616d650000000012000000000000000900"
    "0000617070436c617373000000001200000000000000070000006e50726f"
    "6373000003000000080000006d616e616765640008000000070000006c6f"
    "636b6564000008000000060000006572726f7200000008000000";
    _marshaller_FoamXServer_CaseDescriptor = new _Marshaller_FoamXServer_CaseDescriptor;
    FoamXServer::_tc_CaseDescriptorList = 
    "010000001500000040010000010000002700000049444c3a466f616d5853"
    "65727665722f4361736544657363726970746f724c6973743a312e300000"
    "130000004361736544657363726970746f724c697374000013000000f000"
    "0000010000000f000000e0000000010000002300000049444c3a466f616d"
    "585365727665722f4361736544657363726970746f723a312e3000000f00"
    "00004361736544657363726970746f7200000800000008000000726f6f74"
    "4469720012000000000000000b000000726177526f6f7444697200001200"
    "00000000000009000000636173654e616d65000000001200000000000000"
    "09000000617070436c617373000000001200000000000000070000006e50"
    "726f6373000003000000080000006d616e61676564000800000007000000"
    "6c6f636b6564000008000000060000006572726f72000000080000000000"
    "0000";
    FoamXServer::_tc_JobID = 
    "010000000f00000060000000010000001a00000049444c3a466f616d5853"
    "65727665722f4a6f6249443a312e30000000060000004a6f624944000000"
    "0200000009000000686f73744e616d650000000012000000000000000a00"
    "000070726f63657373494400000003000000";
    _marshaller_FoamXServer_JobID = new _Marshaller_FoamXServer_JobID;
    FoamXServer::_tc_JobIDList = 
    "0100000015000000b0000000010000001e00000049444c3a466f616d5853"
    "65727665722f4a6f6249444c6973743a312e300000000a0000004a6f6249"
    "444c6973740000001300000070000000010000000f000000600000000100"
    "00001a00000049444c3a466f616d585365727665722f4a6f6249443a312e"
    "30000000060000004a6f6249440000000200000009000000686f73744e61"
    "6d650000000012000000000000000a00000070726f636573734944000000"
    "0300000000000000";
    FoamXServer::_tc_JobDescriptor = 
    "010000000f0000009c030000010000002200000049444c3a466f616d5853"
    "65727665722f4a6f6244657363726970746f723a312e300000000e000000"
    "4a6f6244657363726970746f7200000013000000060000006a6f62494400"
    "00000f00000060000000010000001a00000049444c3a466f616d58536572"
    "7665722f4a6f6249443a312e30000000060000004a6f6249440000000200"
    "000009000000686f73744e616d650000000012000000000000000a000000"
    "70726f636573734944000000030000000500000070706964000000000300"
    "0000050000007067696400000000030000000a0000007374617274446174"
    "6500000012000000000000000a000000737461727454696d650000001200"
    "00000000000009000000757365724e616d65000000001200000000000000"
    "0c000000666f616d56657273696f6e00120000000000000005000000636f"
    "6465000000001200000000000000080000006172674c6973740012000000"
    "000000000b00000063757272656e74446972000012000000000000000800"
    "0000726f6f7444697200120000000000000009000000636173654e616d65"
    "000000001200000000000000070000006e50726f63730000030000000700"
    "0000736c61766573000015000000b0000000010000001e00000049444c3a"
    "466f616d585365727665722f4a6f6249444c6973743a312e300000000a00"
    "00004a6f6249444c6973740000001300000070000000010000000f000000"
    "60000000010000001a00000049444c3a466f616d585365727665722f4a6f"
    "6249443a312e30000000060000004a6f6249440000000200000009000000"
    "686f73744e616d650000000012000000000000000a00000070726f636573"
    "73494400000003000000000000000e0000006e436f756e74656450726f63"
    "73000000030000000800000063707554696d65000700000008000000656e"
    "644461746500120000000000000008000000656e6454696d650012000000"
    "0000000007000000737461747573000011000000c0000000010000001e00"
    "000049444c3a466f616d585365727665722f4a6f625374617475733a312e"
    "300000000a0000004a6f62537461747573000000070000000e0000004a4f"
    "425f554e444546494e45440000000e0000004a4f425f4c41554e4348494e"
    "470000000c0000004a4f425f52554e4e494e47000d0000004a4f425f5354"
    "4f5050494e47000000000e0000004a4f425f53555350454e444544000000"
    "0d0000004a4f425f46494e4953484544000000000c0000004a4f425f4142"
    "4f5254454400";
    _marshaller_FoamXServer_JobDescriptor = new _Marshaller_FoamXServer_JobDescriptor;
    FoamXServer::_tc_JobDescriptorList = 
    "0100000015000000fc030000010000002600000049444c3a466f616d5853"
    "65727665722f4a6f6244657363726970746f724c6973743a312e30000000"
    "120000004a6f6244657363726970746f724c69737400000013000000ac03"
    "0000010000000f0000009c030000010000002200000049444c3a466f616d"
    "585365727665722f4a6f6244657363726970746f723a312e300000000e00"
    "00004a6f6244657363726970746f7200000013000000060000006a6f6249"
    "440000000f00000060000000010000001a00000049444c3a466f616d5853"
    "65727665722f4a6f6249443a312e30000000060000004a6f624944000000"
    "0200000009000000686f73744e616d650000000012000000000000000a00"
    "000070726f63657373494400000003000000050000007070696400000000"
    "03000000050000007067696400000000030000000a000000737461727444"
    "61746500000012000000000000000a000000737461727454696d65000000"
    "120000000000000009000000757365724e616d6500000000120000000000"
    "00000c000000666f616d56657273696f6e00120000000000000005000000"
    "636f6465000000001200000000000000080000006172674c697374001200"
    "0000000000000b00000063757272656e7444697200001200000000000000"
    "08000000726f6f7444697200120000000000000009000000636173654e61"
    "6d65000000001200000000000000070000006e50726f6373000003000000"
    "07000000736c61766573000015000000b0000000010000001e0000004944"
    "4c3a466f616d585365727665722f4a6f6249444c6973743a312e30000000"
    "0a0000004a6f6249444c6973740000001300000070000000010000000f00"
    "000060000000010000001a00000049444c3a466f616d585365727665722f"
    "4a6f6249443a312e30000000060000004a6f624944000000020000000900"
    "0000686f73744e616d650000000012000000000000000a00000070726f63"
    "657373494400000003000000000000000e0000006e436f756e7465645072"
    "6f6373000000030000000800000063707554696d65000700000008000000"
    "656e644461746500120000000000000008000000656e6454696d65001200"
    "00000000000007000000737461747573000011000000c000000001000000"
    "1e00000049444c3a466f616d585365727665722f4a6f625374617475733a"
    "312e300000000a0000004a6f62537461747573000000070000000e000000"
    "4a4f425f554e444546494e45440000000e0000004a4f425f4c41554e4348"
    "494e470000000c0000004a4f425f52554e4e494e47000d0000004a4f425f"
    "53544f5050494e47000000000e0000004a4f425f53555350454e44454400"
    "00000d0000004a4f425f46494e4953484544000000000c0000004a4f425f"
    "41424f525445440000000000";
    FoamXServer::_tc_DimensionSet = 
    "010000000f000000c4000000010000002100000049444c3a466f616d5853"
    "65727665722f44696d656e73696f6e5365743a312e30000000000d000000"
    "44696d656e73696f6e5365740000000007000000050000006d6173730000"
    "000007000000070000006c656e6774680000070000000500000074696d65"
    "00000000070000000c00000074656d706572617475726500070000000600"
    "00006d6f6c6573000000070000000800000063757272656e740007000000"
    "120000006c756d696e6f7573496e74656e7369747900000007000000";
    _marshaller_FoamXServer_DimensionSet = new _Marshaller_FoamXServer_DimensionSet;
    FoamXServer::_tc_DimensionSetList = 
    "010000001500000024010000010000002500000049444c3a466f616d5853"
    "65727665722f44696d656e73696f6e5365744c6973743a312e3000000000"
    "1100000044696d656e73696f6e5365744c6973740000000013000000d400"
    "0000010000000f000000c4000000010000002100000049444c3a466f616d"
    "585365727665722f44696d656e73696f6e5365743a312e30000000000d00"
    "000044696d656e73696f6e5365740000000007000000050000006d617373"
    "0000000007000000070000006c656e677468000007000000050000007469"
    "6d6500000000070000000c00000074656d70657261747572650007000000"
    "060000006d6f6c6573000000070000000800000063757272656e74000700"
    "0000120000006c756d696e6f7573496e74656e7369747900000007000000"
    "00000000";
    FoamXServer::_tc_SurfacePatch = 
    "010000000f00000094000000010000002100000049444c3a466f616d5853"
    "65727665722f5375726661636550617463683a312e30000000000d000000"
    "5375726661636550617463680000000004000000050000006e616d650000"
    "000012000000000000000e00000067656f6d657472696354797065000000"
    "12000000000000000500000073697a650000000003000000060000007374"
    "61727400000003000000";
    _marshaller_FoamXServer_SurfacePatch = new _Marshaller_FoamXServer_SurfacePatch;
    FoamXServer::_tc_SurfacePatchList = 
    "0100000015000000f4000000010000002500000049444c3a466f616d5853"
    "65727665722f5375726661636550617463684c6973743a312e3000000000"
    "110000005375726661636550617463684c6973740000000013000000a400"
    "0000010000000f00000094000000010000002100000049444c3a466f616d"
    "585365727665722f5375726661636550617463683a312e30000000000d00"
    "00005375726661636550617463680000000004000000050000006e616d65"
    "0000000012000000000000000e00000067656f6d65747269635479706500"
    "000012000000000000000500000073697a65000000000300000006000000"
    "73746172740000000300000000000000";
    FoamXServer::_tc_FoamXError = 
    "010000001600000090010000010000001f00000049444c3a466f616d5853"
    "65727665722f466f616d584572726f723a312e3000000b000000466f616d"
    "584572726f720000050000000a0000006572726f72436f64650000001100"
    "0000dd000000010000001e00000049444c3a466f616d585365727665722f"
    "4572726f72436f64653a312e300000000a0000004572726f72436f646500"
    "00000900000005000000535f4f4b0000000007000000455f4641494c0000"
    "07000000455f464f414d00000e000000455f494e56414c49445f41524700"
    "00000e000000455f494e56414c49445f5054520000000e000000455f494e"
    "56414c49445f52454600000016000000455f494e4445585f4f55545f4f46"
    "5f424f554e44530000000f000000455f554e4b4e4f574e5f4e414d450000"
    "0d000000455f554e4558504543544544000000000d0000006572726f724d"
    "6573736167650000000012000000000000000b0000006d6574686f644e61"
    "6d65000012000000000000000900000066696c654e616d65000000001200"
    "000000000000070000006c696e654e6f000003000000";
    _marshaller_FoamXServer_FoamXError = new _Marshaller_FoamXServer_FoamXError;
    FoamXServer::_tc_FoamXSYSError = 
    "0100000016000000b0010000010000002200000049444c3a466f616d5853"
    "65727665722f466f616d585359534572726f723a312e300000000e000000"
    "466f616d585359534572726f72000000060000000a0000006572726f7243"
    "6f646500000011000000dd000000010000001e00000049444c3a466f616d"
    "585365727665722f4572726f72436f64653a312e300000000a0000004572"
    "726f72436f64650000000900000005000000535f4f4b0000000007000000"
    "455f4641494c000007000000455f464f414d00000e000000455f494e5641"
    "4c49445f4152470000000e000000455f494e56414c49445f505452000000"
    "0e000000455f494e56414c49445f52454600000016000000455f494e4445"
    "585f4f55545f4f465f424f554e44530000000f000000455f554e4b4e4f57"
    "4e5f4e414d4500000d000000455f554e4558504543544544000000000d00"
    "00006572726f724d65737361676500000000120000000000000009000000"
    "686f73744e616d650000000012000000000000000b0000006d6574686f64"
    "4e616d65000012000000000000000900000066696c654e616d6500000000"
    "1200000000000000070000006c696e654e6f000003000000";
    _marshaller_FoamXServer_FoamXSYSError = new _Marshaller_FoamXServer_FoamXSYSError;
    FoamXServer::_tc_ValidationError = 
    "010000001600000070010000010000002400000049444c3a466f616d5853"
    "65727665722f56616c69646174696f6e4572726f723a312e300010000000"
    "56616c69646174696f6e4572726f7200030000000a0000006572726f7243"
    "6f646500000011000000dd000000010000001e00000049444c3a466f616d"
    "585365727665722f4572726f72436f64653a312e300000000a0000004572"
    "726f72436f64650000000900000005000000535f4f4b0000000007000000"
    "455f4641494c000007000000455f464f414d00000e000000455f494e5641"
    "4c49445f4152470000000e000000455f494e56414c49445f505452000000"
    "0e000000455f494e56414c49445f52454600000016000000455f494e4445"
    "585f4f55545f4f465f424f554e44530000000f000000455f554e4b4e4f57"
    "4e5f4e414d4500000d000000455f554e4558504543544544000000000d00"
    "00006572726f724d65737361676500000000120000000000000009000000"
    "6974656d50617468000000001200000000000000";
    _marshaller_FoamXServer_ValidationError = new _Marshaller_FoamXServer_ValidationError;
    FoamXServer::_tc_FoamXIOError = 
    "0100000016000000e4000000010000002100000049444c3a466f616d5853"
    "65727665722f466f616d58494f4572726f723a312e30000000000d000000"
    "466f616d58494f4572726f7200000000070000000d0000006572726f724d"
    "6573736167650000000012000000000000000b000000696f46696c654e61"
    "6d65000012000000000000000e000000696f53746172744c696e654e6f00"
    "0000030000000c000000696f456e644c696e654e6f00030000000b000000"
    "6d6574686f644e616d65000012000000000000000900000066696c654e61"
    "6d65000000001200000000000000070000006c696e654e6f000003000000"
    ;
    _marshaller_FoamXServer_FoamXIOError = new _Marshaller_FoamXServer_FoamXIOError;
    FoamXServer::PatchToolServer::_tc_IPatchToolServer = 
    "010000000e00000055000000010000003500000049444c3a466f616d5853"
    "65727665722f5061746368546f6f6c5365727665722f495061746368546f"
    "6f6c5365727665723a312e300000000011000000495061746368546f6f6c"
    "53657276657200";
    _marshaller_FoamXServer_PatchToolServer_IPatchToolServer = new _Marshaller_FoamXServer_PatchToolServer_IPatchToolServer;
    _marshaller__seq__seq_long = new _Marshaller__seq__seq_long;
    _marshaller__a3_float = new _Marshaller__a3_float;
    _marshaller__seq_FoamXServer_FoamXAny = new _Marshaller__seq_FoamXServer_FoamXAny;
    _marshaller__seq_FoamXServer_StringPair = new _Marshaller__seq_FoamXServer_StringPair;
    _marshaller__seq_FoamXServer_HostDescriptor = new _Marshaller__seq_FoamXServer_HostDescriptor;
    _marshaller__seq_FoamXServer_ApplicationClassDescriptor = new _Marshaller__seq_FoamXServer_ApplicationClassDescriptor;
    _marshaller__seq_FoamXServer_CaseDescriptor = new _Marshaller__seq_FoamXServer_CaseDescriptor;
    _marshaller__seq_FoamXServer_JobID = new _Marshaller__seq_FoamXServer_JobID;
    _marshaller__seq_FoamXServer_JobDescriptor = new _Marshaller__seq_FoamXServer_JobDescriptor;
    _marshaller__seq_FoamXServer_DimensionSet = new _Marshaller__seq_FoamXServer_DimensionSet;
    _marshaller__seq_FoamXServer_SurfacePatch = new _Marshaller__seq_FoamXServer_SurfacePatch;
  }

  ~__tc_init_PATCHTOOL()
  {
    delete static_cast<_Marshaller_FoamXServer_FoamXType*>(_marshaller_FoamXServer_FoamXType);
    delete static_cast<_Marshaller_FoamXServer_ErrorCode*>(_marshaller_FoamXServer_ErrorCode);
    delete static_cast<_Marshaller_FoamXServer_JobStatus*>(_marshaller_FoamXServer_JobStatus);
    delete static_cast<_Marshaller_FoamXServer_MessageType*>(_marshaller_FoamXServer_MessageType);
    delete static_cast<_Marshaller_FoamXServer_FoamXAny*>(_marshaller_FoamXServer_FoamXAny);
    delete static_cast<_Marshaller_FoamXServer_StringPair*>(_marshaller_FoamXServer_StringPair);
    delete static_cast<_Marshaller_FoamXServer_HostDescriptor*>(_marshaller_FoamXServer_HostDescriptor);
    delete static_cast<_Marshaller_FoamXServer_ApplicationClassDescriptor*>(_marshaller_FoamXServer_ApplicationClassDescriptor);
    delete static_cast<_Marshaller_FoamXServer_CaseDescriptor*>(_marshaller_FoamXServer_CaseDescriptor);
    delete static_cast<_Marshaller_FoamXServer_JobID*>(_marshaller_FoamXServer_JobID);
    delete static_cast<_Marshaller_FoamXServer_JobDescriptor*>(_marshaller_FoamXServer_JobDescriptor);
    delete static_cast<_Marshaller_FoamXServer_DimensionSet*>(_marshaller_FoamXServer_DimensionSet);
    delete static_cast<_Marshaller_FoamXServer_SurfacePatch*>(_marshaller_FoamXServer_SurfacePatch);
    delete static_cast<_Marshaller_FoamXServer_FoamXError*>(_marshaller_FoamXServer_FoamXError);
    delete static_cast<_Marshaller_FoamXServer_FoamXSYSError*>(_marshaller_FoamXServer_FoamXSYSError);
    delete static_cast<_Marshaller_FoamXServer_ValidationError*>(_marshaller_FoamXServer_ValidationError);
    delete static_cast<_Marshaller_FoamXServer_FoamXIOError*>(_marshaller_FoamXServer_FoamXIOError);
    delete static_cast<_Marshaller_FoamXServer_PatchToolServer_IPatchToolServer*>(_marshaller_FoamXServer_PatchToolServer_IPatchToolServer);
    delete static_cast<_Marshaller__seq__seq_long*>(_marshaller__seq__seq_long);
    delete static_cast<_Marshaller__a3_float*>(_marshaller__a3_float);
    delete static_cast<_Marshaller__seq_FoamXServer_FoamXAny*>(_marshaller__seq_FoamXServer_FoamXAny);
    delete static_cast<_Marshaller__seq_FoamXServer_StringPair*>(_marshaller__seq_FoamXServer_StringPair);
    delete static_cast<_Marshaller__seq_FoamXServer_HostDescriptor*>(_marshaller__seq_FoamXServer_HostDescriptor);
    delete static_cast<_Marshaller__seq_FoamXServer_ApplicationClassDescriptor*>(_marshaller__seq_FoamXServer_ApplicationClassDescriptor);
    delete static_cast<_Marshaller__seq_FoamXServer_CaseDescriptor*>(_marshaller__seq_FoamXServer_CaseDescriptor);
    delete static_cast<_Marshaller__seq_FoamXServer_JobID*>(_marshaller__seq_FoamXServer_JobID);
    delete static_cast<_Marshaller__seq_FoamXServer_JobDescriptor*>(_marshaller__seq_FoamXServer_JobDescriptor);
    delete static_cast<_Marshaller__seq_FoamXServer_DimensionSet*>(_marshaller__seq_FoamXServer_DimensionSet);
    delete static_cast<_Marshaller__seq_FoamXServer_SurfacePatch*>(_marshaller__seq_FoamXServer_SurfacePatch);
  }
};

static __tc_init_PATCHTOOL __init_PATCHTOOL;

//--------------------------------------------------------
//  Implementation of skeletons
//--------------------------------------------------------

// PortableServer Skeleton Class for interface FoamXServer::PatchToolServer::IPatchToolServer
POA_FoamXServer::PatchToolServer::IPatchToolServer::~IPatchToolServer()
{
}

::FoamXServer::PatchToolServer::IPatchToolServer_ptr
POA_FoamXServer::PatchToolServer::IPatchToolServer::_this ()
{
  CORBA::Object_var obj = PortableServer::ServantBase::_this();
  return ::FoamXServer::PatchToolServer::IPatchToolServer::_narrow (obj);
}

CORBA::Boolean
POA_FoamXServer::PatchToolServer::IPatchToolServer::_is_a (const char * repoid)
{
  if (strcmp (repoid, "IDL:FoamXServer/PatchToolServer/IPatchToolServer:1.0") == 0) {
    return TRUE;
  }
  return FALSE;
}

CORBA::InterfaceDef_ptr
POA_FoamXServer::PatchToolServer::IPatchToolServer::_get_interface ()
{
  CORBA::InterfaceDef_ptr ifd = PortableServer::ServantBase::_get_interface ("IDL:FoamXServer/PatchToolServer/IPatchToolServer:1.0");

  if (CORBA::is_nil (ifd)) {
    mico_throw (CORBA::OBJ_ADAPTER (0, CORBA::COMPLETED_NO));
  }

  return ifd;
}

CORBA::RepositoryId
POA_FoamXServer::PatchToolServer::IPatchToolServer::_primary_interface (const PortableServer::ObjectId &, PortableServer::POA_ptr)
{
  return CORBA::string_dup ("IDL:FoamXServer/PatchToolServer/IPatchToolServer:1.0");
}

CORBA::Object_ptr
POA_FoamXServer::PatchToolServer::IPatchToolServer::_make_stub (PortableServer::POA_ptr poa, CORBA::Object_ptr obj)
{
  return new ::FoamXServer::PatchToolServer::IPatchToolServer_stub_clp (poa, obj);
}

bool
POA_FoamXServer::PatchToolServer::IPatchToolServer::dispatch (CORBA::StaticServerRequest_ptr __req)
{
  #ifdef HAVE_EXCEPTIONS
  try {
  #endif
    switch (mico_string_hash (__req->op_name(), 53)) {
    case 0:
      if( strcmp( __req->op_name(), "readTriSurface" ) == 0 ) {
        CORBA::String_var _par_fileName;
        CORBA::StaticAny _sa_fileName( CORBA::_stc_string, &_par_fileName._for_demarshal() );

        __req->add_in_arg( &_sa_fileName );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          readTriSurface( _par_fileName.inout() );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        } catch( ::FoamXServer::FoamXIOError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      break;
    case 1:
      if( strcmp( __req->op_name(), "triangulateLocal" ) == 0 ) {
        CORBA::Long _par_startFaceI;
        CORBA::StaticAny _sa_startFaceI( CORBA::_stc_long, &_par_startFaceI );
        CORBA::Long _par_nFaces;
        CORBA::StaticAny _sa_nFaces( CORBA::_stc_long, &_par_nFaces );
        CORBA::Long _par_totalNTris;
        CORBA::StaticAny _sa_totalNTris( CORBA::_stc_long, &_par_totalNTris );
        ::FoamXServer::LongList* _par_triVerts;
        CORBA::StaticAny _sa_triVerts( CORBA::_stcseq_long );
        ::FoamXServer::LongList* _par_localToGlobal;
        CORBA::StaticAny _sa_localToGlobal( CORBA::_stcseq_long );

        __req->add_in_arg( &_sa_startFaceI );
        __req->add_in_arg( &_sa_nFaces );
        __req->add_in_arg( &_sa_totalNTris );
        __req->add_out_arg( &_sa_triVerts );
        __req->add_out_arg( &_sa_localToGlobal );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          triangulateLocal( _par_startFaceI, _par_nFaces, _par_totalNTris, _par_triVerts, _par_localToGlobal );
          _sa_triVerts.value( CORBA::_stcseq_long, _par_triVerts );
          _sa_localToGlobal.value( CORBA::_stcseq_long, _par_localToGlobal );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        delete _par_triVerts;
        delete _par_localToGlobal;
        return true;
      }
      break;
    case 5:
      if( strcmp( __req->op_name(), "getTime" ) == 0 ) {
        char* _res;
        CORBA::StaticAny __res( CORBA::_stc_string, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          _res = getTime();
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        } catch( ::FoamXServer::FoamXIOError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        CORBA::string_free( _res );
        return true;
      }
      break;
    case 6:
      if( strcmp( __req->op_name(), "getNFaces" ) == 0 ) {
        CORBA::Long _res;
        CORBA::StaticAny __res( CORBA::_stc_long, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          _res = getNFaces();
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      break;
    case 8:
      if( strcmp( __req->op_name(), "getNTris" ) == 0 ) {
        CORBA::Long _par_startFaceI;
        CORBA::StaticAny _sa_startFaceI( CORBA::_stc_long, &_par_startFaceI );
        CORBA::Long _par_nFaces;
        CORBA::StaticAny _sa_nFaces( CORBA::_stc_long, &_par_nFaces );
        ::FoamXServer::LongList* _par_nTris;
        CORBA::StaticAny _sa_nTris( CORBA::_stcseq_long );

        CORBA::Long _res;
        CORBA::StaticAny __res( CORBA::_stc_long, &_res );
        __req->add_in_arg( &_sa_startFaceI );
        __req->add_in_arg( &_sa_nFaces );
        __req->add_out_arg( &_sa_nTris );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          _res = getNTris( _par_startFaceI, _par_nFaces, _par_nTris );
          _sa_nTris.value( CORBA::_stcseq_long, _par_nTris );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        delete _par_nTris;
        return true;
      }
      break;
    case 10:
      if( strcmp( __req->op_name(), "changePatchType" ) == 0 ) {
        CORBA::String_var _par_patchName;
        CORBA::StaticAny _sa_patchName( CORBA::_stc_string, &_par_patchName._for_demarshal() );
        CORBA::String_var _par_patchType;
        CORBA::StaticAny _sa_patchType( CORBA::_stc_string, &_par_patchType._for_demarshal() );

        __req->add_in_arg( &_sa_patchName );
        __req->add_in_arg( &_sa_patchType );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          changePatchType( _par_patchName.inout(), _par_patchType.inout() );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      break;
    case 11:
      if( strcmp( __req->op_name(), "getFeatureEdges" ) == 0 ) {
        ::FoamXServer::LongList* _par_edgeLabels;
        CORBA::StaticAny _sa_edgeLabels( CORBA::_stcseq_long );

        __req->add_out_arg( &_sa_edgeLabels );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          getFeatureEdges( _par_edgeLabels );
          _sa_edgeLabels.value( CORBA::_stcseq_long, _par_edgeLabels );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        delete _par_edgeLabels;
        return true;
      }
      break;
    case 12:
      if( strcmp( __req->op_name(), "write" ) == 0 ) {

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          write();
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        } catch( ::FoamXServer::FoamXIOError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      break;
    case 13:
      if( strcmp( __req->op_name(), "getFeatureToEdge" ) == 0 ) {
        ::FoamXServer::LongList* _par_edgeLabels;
        CORBA::StaticAny _sa_edgeLabels( CORBA::_stcseq_long );

        __req->add_out_arg( &_sa_edgeLabels );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          getFeatureToEdge( _par_edgeLabels );
          _sa_edgeLabels.value( CORBA::_stcseq_long, _par_edgeLabels );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        delete _par_edgeLabels;
        return true;
      }
      break;
    case 15:
      if( strcmp( __req->op_name(), "read" ) == 0 ) {

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          read();
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        } catch( ::FoamXServer::FoamXIOError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      if( strcmp( __req->op_name(), "getEdgeToFeature" ) == 0 ) {
        ::FoamXServer::LongList* _par_featLabels;
        CORBA::StaticAny _sa_featLabels( CORBA::_stcseq_long );

        __req->add_out_arg( &_sa_featLabels );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          getEdgeToFeature( _par_featLabels );
          _sa_featLabels.value( CORBA::_stcseq_long, _par_featLabels );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        delete _par_featLabels;
        return true;
      }
      break;
    case 16:
      if( strcmp( __req->op_name(), "addPatch" ) == 0 ) {
        CORBA::String_var _par_patchName;
        CORBA::StaticAny _sa_patchName( CORBA::_stc_string, &_par_patchName._for_demarshal() );

        __req->add_in_arg( &_sa_patchName );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          addPatch( _par_patchName.inout() );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      break;
    case 17:
      if( strcmp( __req->op_name(), "setExtraEdges" ) == 0 ) {
        CORBA::Long _par_edgeI;
        CORBA::StaticAny _sa_edgeI( CORBA::_stc_long, &_par_edgeI );

        __req->add_in_arg( &_sa_edgeI );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          setExtraEdges( _par_edgeI );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      if( strcmp( __req->op_name(), "whichPatch" ) == 0 ) {
        CORBA::Long _par_faceI;
        CORBA::StaticAny _sa_faceI( CORBA::_stc_long, &_par_faceI );

        CORBA::Long _res;
        CORBA::StaticAny __res( CORBA::_stc_long, &_res );
        __req->add_in_arg( &_sa_faceI );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          _res = whichPatch( _par_faceI );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      break;
    case 18:
      if( strcmp( __req->op_name(), "_get_caseName" ) == 0 ) {
        char* _res;
        CORBA::StaticAny __res( CORBA::_stc_string, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = caseName();
        __req->write_results();
        CORBA::string_free( _res );
        return true;
      }
      break;
    case 20:
      if( strcmp( __req->op_name(), "writeTriSurface" ) == 0 ) {
        CORBA::String_var _par_fileName;
        CORBA::StaticAny _sa_fileName( CORBA::_stc_string, &_par_fileName._for_demarshal() );

        __req->add_in_arg( &_sa_fileName );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          writeTriSurface( _par_fileName.inout() );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        } catch( ::FoamXServer::FoamXIOError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      if( strcmp( __req->op_name(), "getNEdges" ) == 0 ) {
        CORBA::Long _res;
        CORBA::StaticAny __res( CORBA::_stc_long, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          _res = getNEdges();
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      break;
    case 25:
      if( strcmp( __req->op_name(), "changeFaces" ) == 0 ) {
        ::FoamXServer::LongList _par_patchIDs;
        CORBA::StaticAny _sa_patchIDs( CORBA::_stcseq_long, &_par_patchIDs );
        ::FoamXServer::LongList* _par_oldToNew;
        CORBA::StaticAny _sa_oldToNew( CORBA::_stcseq_long );

        __req->add_in_arg( &_sa_patchIDs );
        __req->add_out_arg( &_sa_oldToNew );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          changeFaces( _par_patchIDs, _par_oldToNew );
          _sa_oldToNew.value( CORBA::_stcseq_long, _par_oldToNew );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        delete _par_oldToNew;
        return true;
      }
      if( strcmp( __req->op_name(), "getNPoints" ) == 0 ) {
        CORBA::Long _par_startFaceI;
        CORBA::StaticAny _sa_startFaceI( CORBA::_stc_long, &_par_startFaceI );
        CORBA::Long _par_nFaces;
        CORBA::StaticAny _sa_nFaces( CORBA::_stc_long, &_par_nFaces );

        CORBA::Long _res;
        CORBA::StaticAny __res( CORBA::_stc_long, &_res );
        __req->add_in_arg( &_sa_startFaceI );
        __req->add_in_arg( &_sa_nFaces );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          _res = getNPoints( _par_startFaceI, _par_nFaces );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      break;
    case 32:
      if( strcmp( __req->op_name(), "getExtraEdges" ) == 0 ) {
        ::FoamXServer::LongList* _par_edgeLabels;
        CORBA::StaticAny _sa_edgeLabels( CORBA::_stcseq_long );

        __req->add_out_arg( &_sa_edgeLabels );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          getExtraEdges( _par_edgeLabels );
          _sa_edgeLabels.value( CORBA::_stcseq_long, _par_edgeLabels );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        delete _par_edgeLabels;
        return true;
      }
      break;
    case 34:
      if( strcmp( __req->op_name(), "close" ) == 0 ) {

        if( !__req->read_args() )
          return true;

        close();
        __req->write_results();
        return true;
      }
      if( strcmp( __req->op_name(), "_get_caseRoot" ) == 0 ) {
        char* _res;
        CORBA::StaticAny __res( CORBA::_stc_string, &_res );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = caseRoot();
        __req->write_results();
        CORBA::string_free( _res );
        return true;
      }
      break;
    case 35:
      if( strcmp( __req->op_name(), "getPoints" ) == 0 ) {
        ::FoamXServer::FloatList* _par_points;
        CORBA::StaticAny _sa_points( CORBA::_stcseq_float );

        __req->add_out_arg( &_sa_points );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          getPoints( _par_points );
          _sa_points.value( CORBA::_stcseq_float, _par_points );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        delete _par_points;
        return true;
      }
      if( strcmp( __req->op_name(), "deletePatch" ) == 0 ) {
        CORBA::String_var _par_patchName;
        CORBA::StaticAny _sa_patchName( CORBA::_stc_string, &_par_patchName._for_demarshal() );

        __req->add_in_arg( &_sa_patchName );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          deletePatch( _par_patchName.inout() );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      if( strcmp( __req->op_name(), "markFaces" ) == 0 ) {
        ::FoamXServer::LongList _par_protectedEdges;
        CORBA::StaticAny _sa_protectedEdges( CORBA::_stcseq_long, &_par_protectedEdges );
        CORBA::Long _par_faceI;
        CORBA::StaticAny _sa_faceI( CORBA::_stc_long, &_par_faceI );
        ::FoamXServer::BoolList* _par_visited;
        CORBA::StaticAny _sa_visited( CORBA::_stcseq_boolean );

        __req->add_in_arg( &_sa_protectedEdges );
        __req->add_in_arg( &_sa_faceI );
        __req->add_out_arg( &_sa_visited );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          markFaces( _par_protectedEdges, _par_faceI, _par_visited );
          _sa_visited.value( CORBA::_stcseq_boolean, _par_visited );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        delete _par_visited;
        return true;
      }
      break;
    case 37:
      if( strcmp( __req->op_name(), "getEdges" ) == 0 ) {
        ::FoamXServer::LongList* _par_verts;
        CORBA::StaticAny _sa_verts( CORBA::_stcseq_long );

        __req->add_out_arg( &_sa_verts );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          getEdges( _par_verts );
          _sa_verts.value( CORBA::_stcseq_long, _par_verts );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        delete _par_verts;
        return true;
      }
      break;
    case 39:
      if( strcmp( __req->op_name(), "_get_availableTimeSteps" ) == 0 ) {
        ::FoamXServer::StringList* _res;
        CORBA::StaticAny __res( CORBA::_stcseq_string );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        _res = availableTimeSteps();
        __res.value( CORBA::_stcseq_string, _res );
        __req->write_results();
        delete _res;
        return true;
      }
      break;
    case 42:
      if( strcmp( __req->op_name(), "getPatches" ) == 0 ) {
        ::FoamXServer::SurfacePatchList* _par_patches;
        CORBA::StaticAny _sa_patches( _marshaller__seq_FoamXServer_SurfacePatch );

        __req->add_out_arg( &_sa_patches );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          getPatches( _par_patches );
          _sa_patches.value( _marshaller__seq_FoamXServer_SurfacePatch, _par_patches );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        delete _par_patches;
        return true;
      }
      break;
    case 43:
      if( strcmp( __req->op_name(), "setTime" ) == 0 ) {
        CORBA::String_var _par_timeName;
        CORBA::StaticAny _sa_timeName( CORBA::_stc_string, &_par_timeName._for_demarshal() );
        CORBA::Long _par_timeIndex;
        CORBA::StaticAny _sa_timeIndex( CORBA::_stc_long, &_par_timeIndex );

        __req->add_in_arg( &_sa_timeName );
        __req->add_in_arg( &_sa_timeIndex );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          setTime( _par_timeName.inout(), _par_timeIndex );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        } catch( ::FoamXServer::FoamXIOError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      if( strcmp( __req->op_name(), "setFeatureEdges" ) == 0 ) {
        CORBA::Double _par_minCos;
        CORBA::StaticAny _sa_minCos( CORBA::_stc_double, &_par_minCos );

        __req->add_in_arg( &_sa_minCos );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          setFeatureEdges( _par_minCos );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      break;
    case 48:
      if( strcmp( __req->op_name(), "getFeatureSegments" ) == 0 ) {
        ::FoamXServer::LongLongList* _par_featureSegments;
        CORBA::StaticAny _sa_featureSegments( _marshaller__seq__seq_long );

        __req->add_out_arg( &_sa_featureSegments );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          getFeatureSegments( _par_featureSegments );
          _sa_featureSegments.value( _marshaller__seq__seq_long, _par_featureSegments );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        delete _par_featureSegments;
        return true;
      }
      break;
    case 49:
      if( strcmp( __req->op_name(), "triangulate" ) == 0 ) {
        CORBA::Long _par_startFaceI;
        CORBA::StaticAny _sa_startFaceI( CORBA::_stc_long, &_par_startFaceI );
        CORBA::Long _par_nFaces;
        CORBA::StaticAny _sa_nFaces( CORBA::_stc_long, &_par_nFaces );
        CORBA::Long _par_totalNTris;
        CORBA::StaticAny _sa_totalNTris( CORBA::_stc_long, &_par_totalNTris );
        ::FoamXServer::LongList* _par_triVerts;
        CORBA::StaticAny _sa_triVerts( CORBA::_stcseq_long );

        __req->add_in_arg( &_sa_startFaceI );
        __req->add_in_arg( &_sa_nFaces );
        __req->add_in_arg( &_sa_totalNTris );
        __req->add_out_arg( &_sa_triVerts );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          triangulate( _par_startFaceI, _par_nFaces, _par_totalNTris, _par_triVerts );
          _sa_triVerts.value( CORBA::_stcseq_long, _par_triVerts );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        delete _par_triVerts;
        return true;
      }
      break;
    case 52:
      if( strcmp( __req->op_name(), "getFeaturePoints" ) == 0 ) {
        ::FoamXServer::FloatList* _par_points;
        CORBA::StaticAny _sa_points( CORBA::_stcseq_float );

        __req->add_out_arg( &_sa_points );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          getFeaturePoints( _par_points );
          _sa_points.value( CORBA::_stcseq_float, _par_points );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        delete _par_points;
        return true;
      }
      if( strcmp( __req->op_name(), "findPatchID" ) == 0 ) {
        CORBA::String_var _par_patchName;
        CORBA::StaticAny _sa_patchName( CORBA::_stc_string, &_par_patchName._for_demarshal() );

        CORBA::Long _res;
        CORBA::StaticAny __res( CORBA::_stc_long, &_res );
        __req->add_in_arg( &_sa_patchName );
        __req->set_result( &__res );

        if( !__req->read_args() )
          return true;

        #ifdef HAVE_EXCEPTIONS
        try {
        #endif
          _res = findPatchID( _par_patchName.inout() );
        #ifdef HAVE_EXCEPTIONS
        } catch( ::FoamXServer::FoamXError_catch &_ex ) {
          __req->set_exception( _ex->_clone() );
          __req->write_results();
          return true;
        }
        #endif
        __req->write_results();
        return true;
      }
      break;
    }
  #ifdef HAVE_EXCEPTIONS
  } catch( CORBA::SystemException_catch &_ex ) {
    __req->set_exception( _ex->_clone() );
    __req->write_results();
    return true;
  } catch( ... ) {
    CORBA::UNKNOWN _ex (CORBA::OMGVMCID | 1, CORBA::COMPLETED_MAYBE);
    __req->set_exception (_ex->_clone());
    __req->write_results ();
    return true;
  }
  #endif

  return false;
}

void
POA_FoamXServer::PatchToolServer::IPatchToolServer::invoke (CORBA::StaticServerRequest_ptr __req)
{
  if (dispatch (__req)) {
      return;
  }

  CORBA::Exception * ex = 
    new CORBA::BAD_OPERATION (0, CORBA::COMPLETED_NO);
  __req->set_exception (ex);
  __req->write_results();
}


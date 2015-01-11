//===-- gala/backend_d3d9.h -------------------------------------*- C++ -*-===//
//
//  Gala
//
//  This file is distributed under the terms described in LICENSE.
//
//  Author(s):
//
//    * Michael Williams <mike@origamicomet.com>
//
//===----------------------------------------------------------------------===//
///
/// \file
/// \brief
///
//===----------------------------------------------------------------------===//

#if BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_WINDOWS__

//============================================================================//

#ifndef _GALA_BACKEND_D3D9_H_
#define _GALA_BACKEND_D3D9_H_

//============================================================================//

#include "gala/backend.h"

//============================================================================//

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

//===----------------------------------------------------------------------===//

/// \copydoc ::gala::backend::init
extern
GALA_PUBLIC
bool gala_backend_init_d3d9(void);

//===----------------------------------------------------------------------===//

/// \copydoc ::gala::backend::num_adapters
extern
GALA_PUBLIC
size_t gala_backend_num_adapters_d3d9(void);

//===----------------------------------------------------------------------===//

/// \copydoc ::gala::backend::adapters
extern
GALA_PUBLIC
const gala_adapter_t **gala_backend_adapters_d3d9(void);

//===----------------------------------------------------------------------===//

#ifdef __cplusplus
}
#endif // __cplusplus

//===----------------------------------------------------------------------===//

#ifdef __cplusplus

//===----------------------------------------------------------------------===//

namespace gala {

//===----------------------------------------------------------------------===//

namespace backend {

/// \copydoc ::gala_backend_init_d3d9
template <>
extern
GALA_PUBLIC
bool init<gala::backends::D3D9>(void);

/// \copydoc ::gala_backend_num_adapters_d3d9
template <>
extern
GALA_PUBLIC
size_t num_adapters<gala::backends::D3D9>(void);

/// \copydoc ::gala_backend_adapters_d3d9
template <>
extern
GALA_PUBLIC
const gala::Adapter **adapters<gala::backends::D3D9>(void);

} // backend

//===----------------------------------------------------------------------===//

} // gala

//===----------------------------------------------------------------------===//

#endif // __cplusplus

//============================================================================//

#endif // _GALA_BACKEND_D3D9_H_

//============================================================================//

#endif // BITBYTE_FOUNDATION_TIER0_SYSTEM == __BITBYTE_FOUNDATION_TIER0_SYSTEM_WINDOWS__

//============================================================================//

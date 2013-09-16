/*
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any
 * means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 */

#include <agl.h>
#include <agl.private.h>

#if (AGL_BACKEND == AGL_BACKEND_OPENGL)
  #if (AGL_PLATFORM == AGL_PLATFORM_WINDOWS)
    #define WIN32_LEAN_AND_MEAN
    #define WIN32_EXTRA_LEAN
    #include <Windows.h>
    #undef WIN32_EXTRA_LEAN
    #undef WIN32_LEAN_AND_MEAN
    #include <GL/GL.h>
  #else
    #error ("Unknown or unsupported platform!")
  #endif
#else
  #error ("Unknown or unsupported backend!")
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* ========================================================================== */
/*  Integration:                                                              */
/*   * Initialization & Deinitalization                                       */
/*   * Errors & Error Handling                                                */
/*   * Memory Management                                                      */
/* ========================================================================== */

/* ==========================================================================
    Initialization & Deinitalization (agl_initialize, agl_deinitialize):
   ========================================================================== */

#if (AGL_BACKEND == AGL_BACKEND_OPENGL)
  #if (AGL_PLATFORM == AGL_PLATFORM_WINDOWS)
    typedef struct _agl_adapter {
      DWORD device_id;
    } _agl_adapter_t;

    typedef struct _agl_output {
      DWORD device_id;
    } _agl_output_t;
  #else
    #error ("Unknown or unsupported platform!")
  #endif
#else
  #error ("Unknown or unsupported backend!")
#endif

void agl_initialize()
{
#if (AGL_BACKEND == AGL_BACKEND_OPENGL)
  #if (AGL_PLATFORM == AGL_PLATFORM_WINDOWS)
    _agl_num_of_adapters = 0; {
      DISPLAY_DEVICE dd;
      memset((void *)&dd, 0, sizeof(DISPLAY_DEVICE));
      dd.cb = sizeof(DISPLAY_DEVICE);
      DWORD device_id = 0;

      while (EnumDisplayDevices(NULL, device_id, &dd, 0)) {
        _agl_num_of_adapters++;
        device_id++;
      }
    }

    _agl_adapters = (agl_adapter_t *)agl_alloc(
      _agl_num_of_adapters * sizeof(agl_adapter_t),
      agl_alignof(agl_adapter_t));

    /* _agl_adapters = */ {
      DISPLAY_DEVICE dd;
      memset((void *)&dd, 0, sizeof(DISPLAY_DEVICE));
      dd.cb = sizeof(DISPLAY_DEVICE);
      DWORD device_id = 0;
      size_t adapter_id = 0;

      while (EnumDisplayDevices(NULL, device_id, &dd, 0)) {
        agl_adapter_t *adapter = &_agl_adapters[adapter_id];
        _agl_adapter_t *adapter_ = (_agl_adapter_t *)agl_alloc(
          sizeof(_agl_adapter_t), agl_alignof(agl_adapter_t));
        adapter_->device_id = device_id;
        adapter->_internal = (uintptr_t)adapter_;

        /* Only the primary adapter is hardware accelerated in Windows. */
        if (dd.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) {
          adapter->primary = true;
          adapter->type = AGL_HARDWARE;
        } else {
          adapter->primary = false;
          adapter->type = AGL_SOFTWARE;
        }

        /* So, uhm, Microshaft uses EnumDisplayDevices to also query for
          monitors. I don't know... */

        adapter->num_of_outputs = 0; {
          DISPLAY_DEVICE mdd;
          memset((void *)&mdd, 0, sizeof(DISPLAY_DEVICE));
          mdd.cb = sizeof(DISPLAY_DEVICE);
          DWORD device_id_ = 0;

          while (EnumDisplayDevices(&dd.DeviceName[0], device_id_, &mdd, 0)) {
            if (!(dd.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER))
              adapter->num_of_outputs++;
          }
        }

        adapter->outputs = (agl_output_t *)agl_alloc(
          adapter->num_of_outputs * sizeof(agl_output_t),
          agl_alignof(agl_output_t));

        /* adapter->outputs = */ {
          DISPLAY_DEVICE mdd;
          memset((void *)&mdd, 0, sizeof(DISPLAY_DEVICE));
          mdd.cb = sizeof(DISPLAY_DEVICE);
          DWORD device_id_ = 0;
          size_t output_id = 0;

          while (EnumDisplayDevices(&dd.DeviceName[0], device_id_, &mdd, 0)) {
            if (dd.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER) {
              device_id_++;
              continue;
            }

            agl_output_t *output = (agl_output_t *)&adapter->outputs[output_id];
            _agl_output_t *output_ = (_agl_output_t *)agl_alloc(
              sizeof(_agl_output_t), agl_alignof(_agl_output_t));
            output_->device_id = device_id_;
            output->_internal = (uintptr_t)output_;

            output->num_of_display_modes = 0; {
              DEVMODE dm;
              memset((void *)&dm, 0, sizeof(DEVMODE));
              dm.dmSize = sizeof(DEVMODE);
              DWORD mode_id = 0;

              while (EnumDisplaySettingsEx(&mdd.DeviceName[0], mode_id, &dm, 0)) {
                if (!(dm.dmDisplayFlags & DM_GRAYSCALE) &&
                    ((dm.dmBitsPerPel == 24) || (dm.dmBitsPerPel == 32)))
                  { output->num_of_display_modes++; }
                mode_id++;
              }
            }

            output->display_modes = (agl_display_mode_t *)agl_alloc(
              output->num_of_display_modes * sizeof(agl_display_mode_t),
              agl_alignof(agl_display_mode_t));

            /* output->display_modes */ {
              DEVMODE dm;
              memset((void *)&dm, 0, sizeof(DEVMODE));
              dm.dmSize = sizeof(DEVMODE);
              DWORD mode_id = 0;
              size_t display_mode_id = 0;

              while (EnumDisplaySettingsEx(&mdd.DeviceName[0], mode_id, &dm, 0)) {
                if ((dm.dmDisplayFlags & DM_GRAYSCALE) ||
                    ((dm.dmBitsPerPel != 24) && (dm.dmBitsPerPel != 32)))
                  { mode_id++; continue; }

                agl_display_mode_t *display_mode =
                  (agl_display_mode_t *)&output->display_modes[display_mode_id];

                switch (dm.dmBitsPerPel) {
                  case 24:
                    display_mode->format = AGL_R8G8B8;
                    break;
                  case 32:
                    display_mode->format = AGL_R8G8B8A8;
                    break;
                }

                display_mode->width = dm.dmPelsWidth;
                display_mode->height = dm.dmPelsHeight;

                switch (dm.dmDisplayFrequency) {
                  case 0: case 1:
                    display_mode->refresh_rate = 0;
                    break;
                  default:
                    display_mode->refresh_rate = 0;
                    break;
                }

                output->num_of_display_modes++;
                mode_id++;
              }
            }

            output_id++;
            device_id_++;
          }
        }

        adapter_id++;
        device_id++;
      }
    }
  #else
    #error ("Unknown or unsupported platform!")
  #endif
#else
  #error ("Unknown or unsupported backend!")
#endif
}

void agl_deinitialize()
{
  for (size_t adapter_id = 0; adapter_id < _agl_num_of_adapters; ++adapter_id) {
    const agl_adapter_t *adapter = &_agl_adapters[adapter_id];

    for (size_t output_id = 0; output_id < adapter->num_of_outputs; ++output_id) {
      const agl_output_t *output = &adapter->outputs[output_id];

      agl_free((void *)output->display_modes);
      if (output->_internal)
        agl_free((void *)output->_internal);
    }

    agl_free((void *)adapter->outputs);
    if (adapter->_internal)
      agl_free((void *)adapter->_internal);
  }

  agl_free((void *)_agl_adapters);

  _agl_num_of_adapters = 0;
  _agl_adapters = NULL;
}

/* ==========================================================================
    Errors (agl_error_t):
   ========================================================================== */

static agl_error_handler_fn _agl_error_handler = NULL;

agl_error_handler_fn agl_error_handler()
{
  return _agl_error_handler;
}

void agl_set_error_handler(
  agl_error_handler_fn handler)
{
  agl_assert(debug, handler != NULL);
  _agl_error_handler = handler;
}

/* ========================================================================== */

void agl_error(
  const agl_err_t error)
{
  agl_assert(debug, _agl_error_handler != NULL);
  _agl_error_handler(error);
  raise(SIGABRT);
}

/* ==========================================================================
    Allocator (agl_allocator_t):
   ========================================================================== */

static agl_allocator_t *_agl_allocator = NULL;

agl_allocator_t *agl_allocator()
{
  return _agl_allocator;
}

void agl_set_allocator(
  agl_allocator_t *allocator)
{
  agl_assert(debug, allocator != NULL);
  _agl_allocator = allocator;
}

/* ========================================================================== */

void *agl_alloc(const size_t num_of_bytes, const size_t alignment) {
  agl_assert(debug, _agl_allocator != NULL);
  void *ptr = _agl_allocator->alloc(_agl_allocator, num_of_bytes, alignment);
  if (!ptr) agl_error(AGL_EOUTOFMEMORY);
  return ptr;
}

void *agl_realloc(void *ptr, const size_t num_of_bytes, const size_t alignment) {
  agl_assert(debug, _agl_allocator != NULL);
  ptr = _agl_allocator->realloc(_agl_allocator, ptr, num_of_bytes, alignment);
  if (!ptr && (num_of_bytes > 0)) agl_error(AGL_EOUTOFMEMORY);
  return ptr;
}

void agl_free(void *ptr) {
  agl_assert(debug, _agl_allocator != NULL);
  agl_assert(paranoid, ptr != NULL);
  _agl_allocator->free(_agl_allocator, ptr);
}

/* ========================================================================== */
/*  Common/Types:                                                             */
/*   * Pixel Formats                                                          */
/* ========================================================================== */

/* ==========================================================================
    Pixel Formats (agl_pixel_format_t):
   ========================================================================== */

/* ... */

/* ========================================================================== */
/*  Infrastructure:                                                           */
/*   * Adapters                                                               */
/*   * Outputs                                                                */
/*   * Display Modes                                                          */
/* ========================================================================== */

/* ==========================================================================
    Adapters (agl_adapter_t):
   ========================================================================== */

size_t _agl_num_of_adapters = 0;
agl_adapter_t *_agl_adapters = NULL;

size_t agl_adapter_count()
{
  return _agl_num_of_adapters;
}

const agl_adapter_t *agl_adapter_get(
  const size_t adapter_id)
{
  agl_assert(debug, adapter_id < _agl_num_of_adapters);
  return &_agl_adapters[adapter_id];
}

const agl_adapter_t *agl_adapter_primary()
{
  for (size_t adapter_id = 0;
       adapter_id < _agl_num_of_adapters;
       ++adapter_id)
  {
    if (_agl_adapters[adapter_id].primary)
      return &_agl_adapters[adapter_id];
  }

  return NULL;
}

const struct agl_output *agl_adapter_primary_output(
  const agl_adapter_t *adapter)
{
  agl_assert(debug, adapter != NULL);

  for (size_t output_id = 0;
       output_id < adapter->num_of_outputs;
       ++output_id)
  {
    if (adapter->outputs[output_id].primary)
      return &adapter->outputs[output_id];
  }

  return NULL;
}

/* ==========================================================================
    Outputs (agl_output_t):
   ========================================================================== */

const struct agl_display_mode *agl_output_find_closest_matching_display_mode(
  const agl_output_t *output,
  const struct agl_display_mode *templ)
{
  agl_assert(debug, output != NULL);
  agl_assert(debug, templ != NULL);

  for (size_t display_mode_id = 0;
       display_mode_id < output->num_of_display_modes;
       ++display_mode_id)
  {
    const struct agl_display_mode *display_mode =
      &output->display_modes[display_mode_id];

    if ((templ->format != AGL_PIXEL_FORMAT_UNKNOWN) &&
        (display_mode->format != templ->format))
      { continue; }
    if ((templ->width != 0) &&
        (display_mode->width != templ->width))
      { continue; }
    if ((templ->height != 0) &&
        (display_mode->height != templ->height))
      { continue; }
    if ((templ->refresh_rate != 0) &&
        (display_mode->refresh_rate != templ->refresh_rate))
      { continue; }

    return display_mode;
  }

  return NULL;
}

/* ==========================================================================
    Display Modes (agl_display_mode_t):
   ========================================================================== */

/* ... */

/* ========================================================================== */
/*  Runtime:                                                                  */
/*   * Contexts                                                               */
/*   * Command Lists                                                          */
/*   * Requests                                                               */
/*   * Resources                                                              */
/* ========================================================================== */

/* ==========================================================================
    Contexts (agl_context_t):
   ========================================================================== */

/* ... */

/* ==========================================================================
    Command Lists (agl_command_list_t):
   ========================================================================== */

struct agl_command *agl_command_list_enqueue(
  agl_command_list_t *command_list,
  const size_t command_len)
{
  agl_assert(paranoid, command_list != NULL);
  while ((command_list->end - command_list->current) < command_len) {
    if (!command_list->exhausted(command_list))
      agl_error(AGL_EOUTOFMEMORY); }
  struct agl_command *cmd = (struct agl_command *)command_list->current;
  command_list->current += command_len;
  return cmd;
}

const struct agl_command *agl_command_list_dequeue(
  const agl_command_list_t *command_list,
  const struct agl_command *cmd)
{
  agl_assert(paranoid, command_list != NULL);
  agl_assert(paranoid, (((uintptr_t)cmd) >= command_list->begin) || (cmd == NULL));
  agl_assert(paranoid, (((uintptr_t)cmd) <= command_list->current) || (cmd == NULL));
  if (cmd == NULL)
    return NULL;
  uintptr_t cmd_ = ((uintptr_t)cmd) + agl_command_length(cmd);
  return ((cmd_ == command_list->current) ? NULL : (const struct agl_command *)cmd_);
}

/* ==========================================================================
    Requests (agl_request_t):
   ========================================================================== */

/* ... */

/* ==========================================================================
    Resources (agl_resource_t):
   ========================================================================== */

agl_resource_t *agl_resource_create(
  const agl_resource_type_t type)
{
  agl_assert(paranoid, type != AGL_RESOURCE_TYPE_UNKNOWN);
  agl_resource_t *resource = (agl_resource_t *)agl_alloc(
    sizeof(agl_resource_t), agl_alignof(agl_resource_t));
  resource->_type = type;
  resource->_ops = 1;
  resource->_internal = ((uintptr_t)NULL);
  return resource;
}

void agl_resource_destroy(
  agl_resource_t *resource)
{
  agl_assert(paranoid, resource != NULL);
  agl_free((void *)resource);
}

/* ========================================================================== */

agl_resource_type_t agl_resource_type(
  const agl_resource_t *resource)
{
  agl_assert(debug, resource != NULL);
  return resource->_type;
}

uint agl_resource_ops(
  const agl_resource_t *resource)
{
  agl_assert(debug, resource != NULL);
  return resource->_ops;
}

/* ========================================================================== */

void agl_resource_queue_for_create(
  agl_resource_t *resource,
  agl_command_list_t *cmds)
{
  agl_assert(paranoid, resource != NULL);
  agl_assert(paranoid, cmds != NULL);
  agl_atomic_incr(&resource->_ops);
  agl_resource_create_cmd_t *cmd = (agl_resource_create_cmd_t *)
    agl_command_list_enqueue(cmds, sizeof(agl_resource_create_cmd_t));
  cmd->cmd.type = AGL_COMMAND_TYPE_RESOURCE_CREATE;
  cmd->resource = resource;
}

void agl_resource_queue_for_destroy(
  agl_resource_t *resource,
  agl_command_list_t *cmds)
{
  agl_assert(paranoid, resource != NULL);
  agl_assert(paranoid, cmds != NULL);
  agl_atomic_incr(&resource->_ops);
  agl_resource_destroy_cmd_t *cmd = (agl_resource_destroy_cmd_t *)
    agl_command_list_enqueue(cmds, sizeof(agl_resource_destroy_cmd_t));
  cmd->cmd.type = AGL_COMMAND_TYPE_RESOURCE_DESTROY;
  cmd->resource = resource;
}

/* ========================================================================== */

bool agl_resource_is_available(
  const agl_resource_t *resource)
{
  agl_assert(debug, resource != NULL);
  return (agl_atomic_compr_and_swap_ptr(
    (volatile uintptr_t *)&resource->_internal,
    ((uintptr_t)NULL), ((uintptr_t)NULL)) != ((uintptr_t)NULL));
}

bool agl_resource_is_reflective(
  const agl_resource_t *resource)
{
  agl_assert(debug, resource != NULL);
  return (agl_atomic_compr_and_swap_ptr(
    (volatile uint *)&resource->_ops,
    0, 0) == 0);
}

/* ========================================================================== */

bool agl_resource_is_available(
  const agl_resource_t *resource)
{
  agl_assert(debug, resource != NULL);
  return (agl_atomic_compr_and_swap_ptr(
    (volatile uintptr_t *)&resource->_internal,
    ((uintptr_t)NULL), ((uintptr_t)NULL)
  ) != ((uintptr_t)NULL));
}

bool agl_resource_is_reflective(
  const agl_resource_t *resource)
{
  agl_assert(debug, resource != NULL);
  return (agl_atomic_compr_and_swap_ptr(
    (volatile uint *)&resource->_ops,
    0, 0
  ) == 0);
}

/* ========================================================================== */
/*  Commands:                                                                 */
/*   * Resource Commands                                                      */
/* ========================================================================== */

/* ==========================================================================
    Commands (agl_command_t):
   ========================================================================== */

size_t agl_command_length(
  const agl_command_t *cmd)
{
  switch (cmd->type) {
    case AGL_COMMAND_TYPE_RESOURCE_CREATE:
      return sizeof(agl_resource_create_cmd_t);
    case AGL_COMMAND_TYPE_RESOURCE_DESTROY:
      return sizeof(agl_resource_destroy_cmd_t);
    default:
      agl_error(AGL_EUNKNOWN);
  }
}

/* ==========================================================================
    Resource Commands:
   ========================================================================== */

/* ... */

#ifdef __cplusplus
}
#endif /* __cplusplus */

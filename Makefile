# ===----------------------------------------------------------------------=== #
#
#  Abstract Graphics Library (AGL)
#
#  This is free and unencumbered software released into the public domain.
#
#  Anyone is free to copy, modify, publish, use, compile, sell, or
#  distribute this software, either in source code form or as a compiled
#  binary, for any purpose, commercial or non-commercial, and by any
#  means.
#
#  In jurisdictions that recognize copyright laws, the author or authors
#  of this software dedicate any and all copyright interest in the
#  software to the public domain. We make this dedication for the benefit
#  of the public at large and to the detriment of our heirs and
#  successors. We intend this dedication to be an overt act of
#  relinquishment in perpetuity of all present and future rights to this
#  software under copyright law.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
#  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
#  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
#  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
#  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
#  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
#  OTHER DEALINGS IN THE SOFTWARE.
#
#  For more information, please refer to <http://unlicense.org/>
#
# ===----------------------------------------------------------------------=== #

ifndef (ROOT)
	ROOT := .
endif

ifndef (PLATFORM)
  PLATFORM := $(shell ./build/platform.sh)
  ifeq ($(PLATFORM),mingw)
  	PLATFORM := windows
  endif
  ifeq ($(PLATFORM),cygwin)
  	$(error Cygwin is not supported!)
  endif
  $(warning No target platform specified, assuming host platform '$(PLATFORM)'.)
endif

ifndef (ARCHITECTURE)
  ARCHITECTURE := $(shell ./build/architecture.sh)
  $(warning No target architecture specified, assuming host architecture '$(ARCHITECTURE)'.)
endif

ifndef (CONFIGURATION)
	CONFIGURATION := debug
  $(warning No configuration specified, assuming default configuration '$(CONFIGURATION)'.)
endif

.PHONY: all

all: agl

test:
	# TODO: Add unit testing.

docs:
	@echo "[DOXYGEN] Doxyfile"
	@doxygen Doxyfile

clean:
	@echo "[RM] $(ROOT)/_build/obj"
	@rm -R -f $(ROOT)/_build/obj
	@echo "[RM] $(ROOT)/_build/lib"
	@rm -R -f $(ROOT)/_build/lib
	@echo "[RM] $(ROOT)/_build/bin"
	@rm -R -f $(ROOT)/_build/bin

include Makefile.$(PLATFORM)

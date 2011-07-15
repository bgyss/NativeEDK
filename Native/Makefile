#
# Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
#

.PHONY: default build test

default: build

build:
	PYTHONUNBUFFERED=1 scons -j4 -k kl

test: build
	$(MAKE) -C Test

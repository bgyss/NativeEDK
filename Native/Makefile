#
# Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
#

.PHONY: default build test

default: build

build:
	PYTHONUNBUFFERED=1 scons -k -j4 kl

test: build
	$(MAKE) -C Test

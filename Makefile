#
# Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
#

.PHONY: default build test

default: build

build:
	$(MAKE) -C Native build

test:
	$(MAKE) -C Native test

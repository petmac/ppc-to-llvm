#! /bin/bash

set -ev

xcodebuild -scheme tests -derivedDataPath build

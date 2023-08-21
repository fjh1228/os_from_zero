#!/bin/bash
echo | awk "{printf(\"%$2\n\",$1)}"
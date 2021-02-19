TEMPLATE = subdirs

SUBDIRS += \
    libtpms \
    examples \
    BoxFilling2D \
#    Tools \

CONFIG += ordered

# Use this config in project setting
# could save the output into one file to find the bugs
#  > %{CurrentProject:NativePath}/log/%{CurrentBuild:Name}-%{CurrentDate:ISO}-%{UUID}.log 2>&1

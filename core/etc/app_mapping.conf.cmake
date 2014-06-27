# sems application mapping
#
# the application mapping defined here is used,
# if application=$(mapping) is set in sems.conf.
#
# the first regular expression which matches on the 
# request URI sets the application that is executed.
#
# format:
#  regexp=>application
#  lines starting with '#' and empty lines are ignored

# standard applications
^.*=>mysql
^refuse=>refuse
#^.*=>standard

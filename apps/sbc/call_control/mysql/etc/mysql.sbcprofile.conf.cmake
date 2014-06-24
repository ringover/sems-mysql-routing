##############################
#   SBC_PROFILE - CC_Mysql   #
##############################
# mysql.sbcprofile.conf sample
############################
#          Routing         #
############################
# If mysql_routing is desactivated set these variables
####
RURI="sip:$tU@192.168.1.52:5060"
To="sip:$tU@192.168.1.52:5060"

############################
#       Relay  RTP         #
############################
enable_rtprelay=yes
rtprelay_force_symmetric_rtp=yes
rtprelay_msgflags_symmetric_rtp=yes
aleg_rtprelay_interface=default

############################
#       Transcodage        #
############################
enable_transcoder=always
callee_codeccaps=pcma,pcmu
transcoder_codecs=pcma,pcmu
prefer_existing_codec_aleg=yes

############################
#         CC_MySQL         #
############################
# Add Mysql addons
###
call_control=mysql
mysql_module=cc_mysql

###
# data to import in the DB
###
mysql_FromUser=$fU
mysql_ToUser=$tU
mysql_FromDomain=$fd
mysql_ToDomain=$td
#mysql_RURI=sip:$ru
mysql_RURI=$ru

############################
#         Filters          #
############################

header_filter=blacklist
header_list=P-App-Param,P-App-Name


![code](https://raw.githubusercontent.com/hosseinpourziyaie/demonware_companion/master/documentation/readme_header.jpg)

# DEMONWARE COMPANION

## What is Demonware 
a private live-service library developed by one of Activision's Teams and its been implemented in every call of duty game since black ops 1(2010)

it handles all of multiplayer lobby services including matchmaking, cloud based user-stats, marketing, commercial and etc. its also involved in user authorization.


## SET OF QOL TOOLS FOR REVERSE ENGINEERING DW
- buffer_deserializer : tool for automatically deserializing either client packets or server replies. it needs decrypted data buffers that can be aquired by intercepting and dumping from dw functions
- data_interceptor : Dynamic-link library that hooks demonware functions to dump lobby payloads [planned to work with iw6,iw7,s1,s2,h1,bo3 and bo4][wip]
- livestorage_dumper : an external program that opens handle to game memory and exports publisher files [planned to work with iw6,iw7,s1,s2,h1,bo3 and bo4][wip]


## Buffer Deserializer
to deserialize buffer dumps simply drag and drop binary file on it. you can use data buffer structure discovery to analyze any payload dump.

![code](https://raw.githubusercontent.com/hosseinpourziyaie/demonware_companion/master/documentation/screenshot_0001.jpg)

![code](https://raw.githubusercontent.com/hosseinpourziyaie/demonware_companion/master/documentation/screenshot_0002.jpg)

## TODO
- Investigate and place Data Interceptor in perfect location to catch most of information about data buffer like serviceID and taskID
- List All of Livestorage PublisherFiles used by games and their global memory location to dump into files

## Compile
Project coded on VS2022 and latest C++ Standards are considered. and it recommended to use VS2019 or higher.

## CREDITS
All of lovely stackoverflow's contributers those help and provide solutions : [here](https://stackoverflow.com/)

## Disclaimer
This software has been created purely for the purposes of academic research. It is not intended to be used to attack other systems. Project maintainers are not responsible or liable for misuse of the software. Use responsibly.



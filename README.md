# Přehled

Tento projekt je implementací hry ant wars. Hra spočívá v tom, že se hráč snaží obsadit všechna mraveniště na mapě postupným vysíláním armády ze svých mravenišť do nepřátelských mravenišť a pokud přemůže pomocí expedice posádku mraveniště, tak dostane mraveniště pod svoji kontrolu. Každý hráč může své mravence vylepšovat. Na výběr má buď globálně přidat útok nebo životy všem svým existujícím mravencům. Pokud každý z těchto vylepšení použil alespoň třikrát, může vylepšit i dosah svých mravenců. Nebo může povýšit a tím vylepšit jednotlivé mravence ve svých mraveništích na kapitána, medika nebo výbušníka. Kapitán vylepšuje útok svým spojencům v dosahu, medik jim přidává životy a výbušník když umře, zabije všechny nepřátelské mravence v dosahu. Hru lze konfigurovat ze souboru a načítá jednak konfigurační soubor, který určí základní dosah, sílu a útok mravenců a zároveň určí, jak významná budou vylepšení vůči těmto atributům a druhak herní mapu. Rozehranou hru lze také uložit. Při spuštění hry si hráč vybírá, pokud chce začit hrát nějakou uloženou hru. Pokud ne, může si vybrat konfiguraci i mapu buď nějakou svojí nebo nějakou výchozí.

## Polymorfismus

Polymorfismus je využit u mravenců, kde je jedna abstraktní třída, která uchovává společné statistiky a funkcionality jako například dostat zásah nebo kontrola, zda mravenec žije. Dědí ji všechny druhy mravenců (medik, kapitán, výbušník a regulár) a například regulár přidává metody promoteAnt. Je využit také u bonusů, které všechny dědí abstraktní bonus a mění chování metody aplicateUpgrade. Dále je využit u reprezentace herní mapy, kde regulární políčka i mraveniště dělí ze společné třídy CPlace, která uchovává souřadnice. Mraveniště si uchovává navíc pole mravenců a statistiky mravenců, kteří se v mraveništi generují.

### Ukázková hra

	################################
	#@@@@@      &&&&&              #
	#@d00@rrrrrr&g41&              #
	#@@@@@      &&&&&              #
	#          rr==  @@@@@         #
	#          r##===@h00@  #      #
	#          r##   @@@@@###      #
	#     &&&&&r%%%%%rrr###        #
	#     &y50&r%B91%r  #          #
	#     &&&&& %%%%%   #          #
	################################
	
	#####################
	#@@@@@      &&&&&   #
	#@d07@      &g45&   #
	#@@@@@      &&&&&   #
	#           ##      #
	#           ##      #
	#           ##      #
	#           %%%%%   #
	#           %B72%   #
	#           %%%%%   #
	#####################

### Ovládání
enter to continue cycle\n
XaY -attack from X to Y\n
up-a -upgrade attack\n
up-h -upgrade health\n
up-r -upgrade range\n
promXc -promote first ant in X to captain\n
promXb -promote first ant in X to bomber\n
promXm -promote first ant in X to medic\n
save -save game\n
exit -exit\n

### Diagram tříd
![diagram trid](class_diagram.png "Diagram")

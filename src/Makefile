clean:
	@ cd Data && rm -f vocab/* content/* html/* database.txt index.txt indegree.txt
	@ cd ..

crawl:
	@ cd Crawling && python3 crawler.py 
	@ cd ..


inv:
	@ cd Inverted && g++ main.cpp djvector.cpp -o main&&./main
	@ cd ..

sch:
	cd Search/ && g++ djsearch.cpp djhelper.cpp main.cpp -o main && ./main
	@ cd ..

run:
	@ cd Data && rm -f content/* html/* database.txt index.txt indegree.txt
	@ cd ..
	@ cd Crawling && python3 crawler.py 
	@ cd ../Inverted && g++ main.cpp djvector.cpp -o main&&./main
	@ cd ../Search/ && g++ djsearch.cpp djhelper.cpp main.cpp -o main && ./main
	@ cd ..

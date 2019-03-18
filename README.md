# road-network-partitioning-for-recommender-systems
Road network partitioning scheme for faster queries in recommender systems.
This project was part of the authors' R&D project during their undergraduate, under the guidance of [Prof. Prosenjit Gupta](https://sites.google.com/site/prosenjitgupta/).

## Motivation
Recommender systems have been successfully used to provide items of interest to the user, from an overwhelming set of choices. 
However, where items being recommended have an associated location, by Tobler's First Law of Geography, there is a general 
preference for things nearby. This aspect of locality can be leveraged not only to improve the quality and accuracy of 
recommendations, but also the scalability of the recommendation algorithm by suitably partitioning the road network used 
for travelling to the places being recommended.


## Partitioning Strategy
Queries of the form - find k POIs nearby, find distances from all the POIs present in the entire region and return the nearest 
k results. Distances to POIs that are very far away and which the user would never travel to, are also calculated. Hence, this 
method turns out to be inefficient. A possible solution to this is to partition the region into smaller grids, locate the user in a 
grid and provide recommendations from the POIs present in that grid. This would improve the running time but decrease the accuracy
in some cases.

<p align="center">
<img height=384 src="https://github.com/piyush-jaiswal/road-network-partitioning-for-recommender-systems/blob/master/docs/unpartitioned.jpg">
</p>

<p align="center"><b>Unpartitoned network</b></p>

<p align="center">
<img height=384 src="https://github.com/piyush-jaiswal/road-network-partitioning-for-recommender-systems/blob/master/docs/partitioned.jpg">
</p>

<p align="center"><b>Partitoned network</b></p>


## Shifting
However, this method gives inaccurate results when the user lies near the boundary of a grid. This is due to the fact that 
the nearest POI could lie in a neighbouring grid, which would never be considered.<br>
To solve this limitation, the case when the user lies near a boundary is detected and the grids are shifted, in order to include the POIs 
that lie in the neighbouring grids. As shifting just horizontally or vertically does not work in all cases (see fig), the grids are 
shifted diagonally.

<p align="center">
<img height=384 src="https://github.com/piyush-jaiswal/road-network-partitioning-for-recommender-systems/blob/master/docs/horizontal_shift.png">
</p>

<br>
<p align="center">
<img height=384 src="https://github.com/piyush-jaiswal/road-network-partitioning-for-recommender-systems/blob/master/docs/diagonal_shift.png">
</p>

## Observation
The partitioning strategy is tested by comparing three approaches:
- Unpartitioned: Original road network without any partitions.
- Paritioned: Partitioned road network.
- Partitioned with shifts: Partitioned road network with shifts.

Let the time taken by an approach be represented as ‘T’ and the accuracy as ‘A’. Then the following is observed:
- T<sub>Unpartitioned</sub> > T<sub>Partitioned with shifts</sub> > T<sub>Partitioned</sub>
- A<sub>Unpartitioned</sub> >= A<sub>Partitioned with shifts</sub> >= A<sub>Partitioned</sub>

<br>
<img src="https://github.com/piyush-jaiswal/road-network-partitioning-for-recommender-systems/blob/master/docs/accuracy.jpg">
where,<br>
r<sub>approach</sub> = Set of POIs returned by an approach<br>
r<sub>optimal</sub> = Set of POIs returned by the Unpartitioned approach


## Dataset
The dataset used for this project was obtained from FourSqaure for the city of New York. This can be found [here](https://sites.google.com/site/yangdingqi/home/foursquare-dataset) under ```NYC and Tokyo Check-in Dataset```. 
Columns used for the project:
- Venue category ID: POI identifier
- Venue category name: Interest identifier
- Latitude
- Longitude

The cleaned dataset can be found in ```data/unique.csv```.

## Dependencies
### node: <br>
```sudo apt-get install nodejs```


## Usage
Distances from user location to the various POIs are obtained using the [Google Distance matrix API](https://developers.google.com/maps/documentation/distance-matrix/start).
The code to consume the web service can be found in ```googleDistanceAPI.js```.<br>
In order to use the API, an API key is required. 
This key can be obtained by following [this](https://developers.google.com/maps/documentation/distance-matrix/get-api-key) tutorial.

- Paste the API key as the value of ```API_KEY``` global variable in ```googleDistanceAPI.js```.
- Type ```make``` to view available commands.
- Type ```make run``` to run the project.

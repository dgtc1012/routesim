# routesim

## setup

```
ssh -Y pittusername@netlab-X.cs.pitt.edu
cd /opt/pittusername/
cp /opt/routelab-f17.tar.gz .
tar -xvzf routelab-f17.tar.gz
cd routelab
```

## build

### distance vector

```
make TYPE=DISTANCEVECTOR clean depend all
```

### link state

```
make TYPE=LINKSTATE clean depend all
```

## run

```
./routesim inputs/topologyfile inputs/eventfile
```

Dannah Gersh Dan McGrath

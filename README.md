# routesim

## setup

```
ssh -Y pittusername@netlab-X.cs.pitt.edu
cd /opt/pittusername/
cp /opt/routelab-f17.tar.gz .
tar -xvzf routelab-f17.tar.gz
cd routelab
```

copy the following files from our submission

```
cp thisfolder/messages.* routelab/
cp thisfolder/node.* routelab/
cp thisfolder/table.* routelab/
cp thisfolder/distancevector.* routelab/
cp thisfolder/linkstate.* routelab/
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

work was pair programmed by 

Dannah Gersh & Dan McGrath

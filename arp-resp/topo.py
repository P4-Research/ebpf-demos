from mininet.topo import Topo
from mininet.net import Mininet
from mininet.log import setLogLevel, info
from mininet.cli import CLI

class DemoTopo(Topo):
    "Demo topology"

    def __init__(self, **opts):
        # Initialize topology and default options
        Topo.__init__(self, **opts)

        h1 = self.addHost('h1',
                      ip="10.0.0.1/24",
                      mac='00:00:00:00:00:01')
        h2 = self.addHost('h2',
                      ip="10.0.0.2/24",
                      mac='00:00:00:00:00:02')

        s1 = self.addSwitch('s1')

        self.addLink(s1, h1)
        self.addLink(s1, h2)


def main():
    topo = DemoTopo()
    net = Mininet(topo=topo,
                  controller=None)

    net.start()
    s1 = net.get('s1')

    s1.cmd('ovs-ofctl add-flow s1 actions=NORMAL')

    CLI(net)

    net.stop()


if __name__ == '__main__':
    setLogLevel('info')
    main()
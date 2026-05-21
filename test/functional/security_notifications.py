#!/usr/bin/env python3
# Copyright (c) 2024-present The Bitcoin Core developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or http://www.opensource.org/licenses/mit-license.php.
"""Test security hardening of notifications."""
from test_framework.test_framework import BitcoinTestFramework
from test_framework.util import (
    assert_equal,
)

class SecurityNotificationsTest(BitcoinTestFramework):
    def set_test_params(self):
        self.num_nodes = 1

    def run_test(self):
        self.log.info("Testing -startupnotify execution")
        startup_file = self.nodes[0].datadir_path / "startup.out"
        # We restart the node with -startupnotify
        self.restart_node(0, extra_args=[f"-startupnotify=echo 'started' > {startup_file}"])
        self.wait_until(lambda: startup_file.exists())
        with open(startup_file, "r") as f:
            assert_equal(f.read().strip(), "started")

if __name__ == '__main__':
    SecurityNotificationsTest(__file__).main()

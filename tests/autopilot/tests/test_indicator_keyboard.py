import autopilot.introspection.gtk
import os
import pyatspi.registry
import pyatspi.utils
import time
import unity.tests

def print_accessible(root, level=0):
    print level * ' ', root

    for node in root:
        print_accessible(node, level + 1)

def get_accessible_with_name_and_role(root, name, role):
    is_accessible = lambda a: a.name == name and a.get_role_name() == role
    return pyatspi.utils.findDescendant(root, is_accessible, True);

def get_panel_accessible(root):
    return get_accessible_with_name_and_role(root, 'unity-panel-service', 'application')

def get_app_menu_accessible(root):
    is_app_menu = lambda a: len(a) > 0 and a[0].name == 'File' and a[0].get_role_name() == 'label'
    return pyatspi.utils.findDescendant(root, is_app_menu, True)

def get_label_accessible_with_name(root, name):
    return get_accessible_with_name_and_role(root, name, 'label')

def get_submenu_accessible(root):
    return root[0]

def get_menu_item_accessible_with_name(root, name):
    is_menu_item = lambda a: a.name == name and a.get_role_name() in ('menu item', 'check menu item', 'radio menu item')
    return pyatspi.utils.findDescendant(root, is_menu_item, True);

def get_accessible_index(root, node):
    for i in xrange(len(root)):
        if root[i] == node:
            return i

    return -1

class IndicatorKeyboardTestCase(unity.tests.UnityTestCase):

    def setUp(self):
        super(IndicatorKeyboardTestCase, self).setUp()

        registry = pyatspi.registry.Registry()
        self.desktop = registry.getDesktop(0)

        # This is needed on systems other than the EN locale
        os.putenv("LC_ALL", "C")
        self.addCleanup(os.unsetenv, "LC_ALL")

    def test_character_map(self):
        print_accessible(self.desktop)

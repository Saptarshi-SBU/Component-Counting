import os
import pandas as pd
from glob import glob
import xml.etree.ElementTree as ET

def correct_path_pascal_voc_xml(data_dir):
    '''
      The recipe updates XML filename & path section
    '''
    xml_list = []
    counter = 0
    for xml_file in glob(data_dir + '/*.xml'):
        tree = ET.parse(xml_file)
        root = tree.getroot()
        file_node = root.find('filename')
        file_name = file_node.text
        file_node.text = os.path.basename(xml_file).replace(".xml", ".jpg")
        path_node = root.find('path')
        path_node.text = xml_file.replace(".xml", ".jpg")
        print(file_node.text)
        tree.write(xml_file)

correct_path_pascal_voc_xml("data/Annotations")

import os

#Создание директории build/
#############################################################
def create_dir(dir_):
    os.mkdir(dir_)

def delete_dir(dir_, file_):
    try:
        os.remove(dir_ + file_)
    except:
        pass
    os.rmdir(dir_)

dir_ = "build/" # имя директории куда будет собираться библиотека
file_ = "cli.hpp" # имя файла куда будет собираться библиотека

try:
    create_dir(dir_)
except FileExistsError:
    delete_dir(dir_, file_)
    create_dir(dir_)
#############################################################
    
#Чтение всех файлов из path и объединение всё в переменную code
#############################################################    
paths = ["src/Flag/Flag.hpp", "src/Flag/Flag.cpp", "src/Command/Command.hpp", "src/Cli/Cli.hpp", 
"src/Cli/Cli.cpp", "src/Command/Command.cpp"] # !Порядок файлов важен (запись в code идёт именно в этом порядке)
code = ""
includes = "#pragma once\n\n" # Собираем сюда все подключаемые библиотеки, чтобы потом в файле поместить их 
сверху
for path in paths:
    file = open(path, "r")
    for str_ in file:
        if ("#" in str_):
            if not("#include \"" in str_ or "#pragma" in str_):
                includes += str_
        else:    
            code += str_
code = includes + code   
#############################################################
    
#Создание файла собранной библиотеки cli.hpp    
#############################################################    
file = open(dir_ + file_, "w+")
file.write(code)    
file.close()
#############################################################

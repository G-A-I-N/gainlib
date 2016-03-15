echo "// Copyright 2014" > temp
git blame -e $1 | awk '{ print $2 }' | sed 's/(//g' | sed 's/<//g' | sed 's/>//g' | sort | uniq -c | awk '{print "//   "$2}' >> temp
echo "// " >> temp
echo "// Licensed under the Apache License, Version 2.0 (the "License");" >> temp
echo "// you may not use this file except in compliance with the License." >> temp
echo "// You may obtain a copy of the License at" >> temp
echo "// " >> temp
echo "//     http://www.apache.org/licenses/LICENSE-2.0" >> temp
echo "// " >> temp
echo "// Unless required by applicable law or agreed to in writing, software" >> temp
echo "// distributed under the License is distributed on an "AS IS" BASIS," >> temp
echo "// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied." >> temp
echo "// See the License for the specific language governing permissions and" >> temp
echo "// limitations under the License." >> temp

cat $1 |  sed '/^\/\*/,/\*\//d' >> temp
mv temp $1
dos2unix $1

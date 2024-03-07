#!/bin/bash
unzip greetings_human.zip

# parse out the email, get the base64 encoded attachment, decode it
cat greetings_human.eml |
    tr -d '\r' |
    tr '\n' '%' |
    sed -re 's|.*the_ai_repository_of_knowledge%(.*)%--.*|\1|' | tr -d '%' | base64 -d > /tmp/sun2023_knowledge_repository_bundle

rm -rf sun2023_knowledge_repository_bundle/
# checkout the bundle
git clone /tmp/sun2023_knowledge_repository_bundle

rm /tmp/sun2023_knowledge_repository_bundle

cd sun2023_knowledge_repository_bundle

declare -A lookup
lookup=(["5fd3cd45a3b670d5704400a71387516549e2aa32"]="W" ["e87ef9840721e1b506edf42075a6c5f87d326299"]="E" ["60af338eb2c164d5bc14ad3bf934e0403f7ea8c7"]="5" ["d1a2bda51e981fdb7c9464a20f3e1a2458392987"]="7" ["334d63c747ff3dcefeba348a09e7a648e6ee9ac5"]="T" ["4dc34055d26f1ffd68e2a45209dab5725353cbec"]="C" ["a509566a50368d7a3d3f752000d532612c777d12"]="G" ["bcc942cbd9daa26f9eac42af35616359ba96c89a"]="=" ["4a4dcd25f19a9cd24bdef0d516126f73c94cffe2"]="I" ["8573814a380081b724f4d194461d27d26279bdfb"]="X" ["ba121e0236d9d94cd7922d11a10f5d4ef3351bc6"]="Q" ["24194ecbe1441198326227107a46657e944777d6"]="S" ["d6ecf9e0db95a25ac7683981012e1fbf55a915dd"]="H" ["a16732f0f46fc0c1a4787c0efe8b3f653b178ccc"]="6" ["ac307d87b796d2ee4df51c55975324fff02482e3"]="P" ["e7d163cee44a326a644778aea8520f8e9f0aa75b"]="B" ["da461d13a56159c7de35d44f62e40d89dfc88816"]="N" ["360b8f2a3aa3d7c3ab47505e2a3b540bf837a57d"]="Z" ["ba35e81561744531dff8d93dbbab4c978fcb4e56"]="M" ["af2e47a9fc65415bce94914d181e27b55f8cdafc"]="D" ["82143e7ec8d41115e55b096e8f11d2c9bb9cf1c1"]="O" ["3b6dfdef48eb571bdbede66bd176350cf9f463fb"]="3" ["130d20387f9f5bade990654364c9e9f037fa043e"]="V" ["4e680a3db2140e1945bbdc30cb11a72e7695b1a0"]="U" ["4411529ecb7e85ab32fb9e65123a4d22e1244105"]="4" ["c8c12e8a889a076f068c3d05e651e84a0a56c55a"]="L" ["c35928e7999553c3b56e2d4b078351d4cb4c1f9f"]="Y" ["b067eb800cc5ec0c0160a1ff01beae4c3075c528"]="A" ["2b87a705c10e6becbfa4155e6cde9d96e512a0fd"]="J" ["8d9b99ce75d4090f0b7aebb83d01cbafee981db1"]="F" ["dac510f32b638314df2765fa6ab8aad5fb1893e1"]="2" ["f649b60b9a68b302b870ca6d3cbd7095e7f00409"]="R" ["a0181f2fbbbfef729813baaae9187ff9341b33c0"]="K")

# go through the repo history, and based on our lookup table, add the current character to the string
string=""
while true; do
    checksum=$(sha1sum ./data | cut --fields=1 --delimiter=' ')
    string+="${lookup[$checksum]}"
    git reset --hard HEAD~1
    if [ $? -ne 0 ]; then
        break;
    fi
done
# reverse the string as we constructed it in reverse
string=$(echo $string | rev)

# decode the base32 string and unzip it, and grep for the flag (the text can be detected with CyberChef's magic mode if the user isn't aware of the encoding)
echo $string | base32 -d | gzip -d | grep -e 'sun{'

cd ..
rm -rf sun2023_knowledge_repository_bundle

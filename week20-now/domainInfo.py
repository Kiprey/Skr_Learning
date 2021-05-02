# encoding=utf-8

import dns.resolver
import requests
import json
import time
import datetime

'''
@brief  检测某个域名是 A 还是 CNAME
@param  domain 目标域名的字符串
@return 字符串 'A' / 'CNAME' / "ALL"，表示该域名的类型
'''
def getDomainType(domain):
    # 获取 DNS resolver
    dns_resolver = dns.resolver.Resolver()
    # 加了几个公共 DNS 进去（不过这行注释掉也可以正常运行）
    dns_resolver.nameservers = ["8.8.8.8", "8.8.4.4", "114.114.114.114", "223.5.5.5"]

    # 获取当前域名的 A 记录
    A_list = []
    try:
        A_answ = dns_resolver.resolve(domain, "A")
        for A in A_answ:
            A_list.append(A)
            print("[getDomainType]\tA: ", A)
    except dns.resolver.NoAnswer:
        pass

    # 获取当前域名的 CNAME 记录
    CNAME_list = []
    try:
        CNAME_answ = dns_resolver.resolve(domain, "CNAME")
        for CNAME in CNAME_answ:
            CNAME_list.append(CNAME)
            print("[getDomainType]\tCNAME: ", CNAME)
    except dns.resolver.NoAnswer:
        pass

    # print(A_list, CNAME_list)

    domain_type = ""
    if len(A_list) == 0 and len(CNAME_list) != 0:
        domain_type = "CNAME"
    elif len(A_list) != 0 and len(CNAME_list) == 0:
        domain_type = "A"
    else:
        domain_type = "ALL"
    return domain_type

'''
@brief  检测某个域名是否过期
@param  domain 目标域名的字符串
@return true/false，表示该域名是否已经过期，或者没有注册
'''
def checkDomainExpired(domain):
    requests_data = {
        "domain": str(domain),
        "type": 'true',
        "standard": 'true'
    }
    # 使用公共 API 查询 Whois 信息 https://api.devopsclub.cn/docs/whoisquery
    # API如果调用太频繁或许、可能会被屏蔽，不过这个API 源码是开源的 https://github.com/spdir/apiTools
    url_api = 'https://api.devopsclub.cn/api/whoisquery'
    whois_info = requests.get(url=url_api, params=requests_data).json()
    # 输出可能产生的错误信息
    if len(whois_info['msg']) != 0:
        print("[checkDomainExpired]\twhois msg: ", whois_info['msg'])

    # 获取当前域名的信息
    domain_data = whois_info['data']

    # 判断域名相关状态
    if domain_data['status'] == 0:
        # 可以成功获取到域名信息
        # 现在开始获取时间 例如获取 "2026-10-11T11:05:17Z"
        expirationTime = domain_data['data']['expirationTime']
        print("[checkDomainExpired]\texpirationTime: ", expirationTime)
        # ISODate时间格式相关参考：https://blog.csdn.net/aerchi/article/details/78717232
        # 注意：获取到的expiration是0时区的
        exp_time = datetime.datetime.strptime(expirationTime, "%Y-%m-%dT%H:%M:%SZ")
        # 因此这里也获取当前时间所对应0时区的时间
        utc_now = datetime.datetime.utcnow()
        # 进行比较，如果时间超过，则表示域名过期
        if exp_time < utc_now:
            return True
        else:
            return False

    elif domain_data['status'] == 1:
        print("[checkDomainExpired]\t域名[" + domain + "]解析失败!")
        return False
    elif domain_data['status'] == 2:
        print("[checkDomainExpired]\t域名未注册")
        return True # 如果域名未注册，则也可以暂认为过期
    elif domain_data['status'] == 3:
        print("[checkDomainExpired]\t暂不支持此域名后缀查询!")
        return False
    elif domain_data['status'] == 4:
        print("[checkDomainExpired]\t域名查询失败!")
        return False
    elif domain_data['status'] == 5:
        print("[checkDomainExpired]\t请求数据错误!")
        return False
    else:
        raise RuntimeError("无法识别的 status")
    # UNREACHABLE    
    return False

'''
@brief  检测某个域名是 A 还是 CNAME，并且查询域名是否过期
@param  domain 目标域名的字符串
@return 一个二元组：
            第一个元素是 'A' / 'CNAME' / 'ALL'，表示该域名的类型
            第二个元素为 true / false，表示该域名是否已经过期
'''
def checkDomain(domain):
    domain_type = getDomainType(domain)
    domain_is_expired = checkDomainExpired(domain)
    return domain_type, domain_is_expired

domain_type, domain_is_expired = checkDomain("map.baidu.com")
print("domain_type:\t", domain_type)
print("domain_is_expired ?\t", domain_is_expired)